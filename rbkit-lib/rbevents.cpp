#include <msgpack.hpp>
#include "subscriber.h"
#include "rbevents.h"
#include "mpparser.h"


static QVariantList parseMsgpackObjectArray(const msgpack::object_array&);
static QVariantMap parseMsgpackObjectMap(const msgpack::object_map&);
static QList<RBKit::EventPtr> parseEventCollection(const QVariantList&);


static QVariant parseMsgpackObject(const msgpack::object& obj)
{
    switch (obj.type) {
    case msgpack::type::ARRAY :
        return QVariant(parseMsgpackObjectArray(obj.via.array));
    case msgpack::type::MAP :
        return QVariant(parseMsgpackObjectMap(obj.via.map));

    case msgpack::type::RAW :
        return QVariant(RBKit::StringUtil::rawToQString(obj));
    case msgpack::type::DOUBLE :
        return QVariant(obj.via.dec);
    case msgpack::type::POSITIVE_INTEGER :
        return QVariant((unsigned long long int)(obj.via.u64));
    case msgpack::type::NIL :
        return QVariant("");

    default:
        qDebug() << "throwing error while parsing event" << obj.type;
        throw "unknown object type";
    }
}

// NOTE: This can be improved with the version that hemant is writing for GCStats.
static QVariantMap parseMsgpackObjectMap(const msgpack::object_map& obj)
{
    QVariantMap map;

    msgpack::object_kv* list = obj.ptr;
    for (uint32_t iter = 0; iter != obj.size; ++iter) {
        msgpack::object key = list->key;
        msgpack::object val = list->val;

        // qDebug() << key.type << val.type;

        QString keyStr = RBKit::StringUtil::rawToQString(key);
        map[keyStr] = parseMsgpackObject(val);

        ++list;
    }

    return map;
}

RBKit::EventDataBase* RBKit::makeEventFromQVariantMap(const QVariantMap &map) {
    QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(map["timestamp"].toULongLong());
    auto eventType = static_cast<RBKit::EventType>( map["event_type"].toInt() );

    RBKit::EventDataBase* event(nullptr);
    switch (eventType) {
    case RBKit::EtObjCreated:
    {
        auto object = RBKit::payloadToObject(map["payload"].toMap());
        event = new RBKit::EvtNewObject(timestamp, eventType, object);
    }
    break;

    case RBKit::EtObjDestroyed:
        // event = new RBKit::EvtDelObject(timestamp, eventType, map["payload"].toMap());
        break;

    case RBKit::EtGcStats:
        event = new RBKit::EvtGcStats(timestamp, eventType, map["payload"].toMap());
        break;

    case RBKit::EtGcStart:
        event = new RBKit::EvtGcStart(timestamp, eventType);
        break;

    case RBKit::EtGcStartM:
        event = new RBKit::EvtGcStartM(timestamp, eventType);
        break;

    case RBKit::EtGcEndS:
        event = new RBKit::EvtGcStop(timestamp, eventType);
        break;

    case RBKit::EtObjectSpaceDump:
    {
        auto objects = RBKit::payloadToObjects(map["payload"].toList());
        event = new RBKit::EvtObjectDump(timestamp, eventType, objects);
    }
    break;

    case RBKit::EtEventCollection:
    {
        auto events = parseEventCollection(map["payload"].toList());
        event = new RBKit::EvtCollection(timestamp, eventType, events);
    }
    break;

    default:
        qDebug() << "Unable to parse event of type: " << eventType;
    }

    return event;
}


static QVariantList parseMsgpackObjectArray(const msgpack::object_array& array)
{
    QVariantList objList;

    for (uint32_t iter = 0; iter != array.size; ++iter) {
        objList.append(parseMsgpackObject(array.ptr[iter]));
    }

    return objList;
}


RBKit::EventDataBase* RBKit::parseEvent(const QByteArray& message)
{
    msgpack::unpacked unpackedMessage;
    msgpack::unpack(&unpackedMessage, message.data(), message.size());

    msgpack::object_map obj = unpackedMessage.get().via.map;

    QVariantMap map = parseMsgpackObjectMap(obj);
    return makeEventFromQVariantMap(map);
}

static QList<RBKit::EventPtr> parseEventCollection(const QVariantList& list)
{
    QList<RBKit::EventPtr> events;

    for (auto& eventMap : list) {
        auto event = RBKit::makeEventFromQVariantMap(eventMap.toMap());
        events.append(RBKit::EventPtr(event));
    }

    return events;
}


// =================================================================================
RBKit::EventDataBase*
RBKit::EventParser::eventFromMsgpackObject(msgpack::object& object) const
{
    auto map = object.as< QMap<unsigned int, msgpack::object> >();

    auto eventType = guessEvent(object);
    auto ts = map[RBKit::EfTimestamp].as<double>();
    auto timestamp = QDateTime::fromMSecsSinceEpoch(ts);
    auto payload = map[RBKit::EfPayload];

    RBKit::EventDataBase* event(nullptr);
    switch (eventType) {
    case RBKit::EtObjCreated:
        event = new RBKit::EvtNewObject(timestamp, eventType,
                                        payload.as< RBKit::ObjectDetailPtr >());
        break;

    case RBKit::EtObjDestroyed:
    {
        auto map = payload.via.map.ptr;
        auto id = (map->val).as< unsigned long long >();
        event = new RBKit::EvtDelObject(timestamp, eventType, id);
    }
    break;

    case RBKit::EtGcStats:
        event = new RBKit::EvtGcStats(timestamp, eventType,
                                      payload.as<QVariantMap>());
        break;

    case RBKit::EtGcStart:
        event = new RBKit::EvtGcStart(timestamp, eventType);
        break;

    case RBKit::EtGcStartM:
        event = new RBKit::EvtGcStartM(timestamp, eventType);
        break;

    case RBKit::EtGcEndS:
        event = new RBKit::EvtGcStop(timestamp, eventType);
        break;

    case RBKit::EtObjectSpaceDump:
        event = new RBKit::EvtObjectDump(timestamp, eventType,
                                         parseObjects(payload));
        break;

    default:
        qDebug() << "Unable to parse event of type: " << eventType;
    }

    return event;
}


QList<RBKit::EventPtr>
RBKit::EventParser::parseEvents(const msgpack::object& objarray) const
{
    QList<RBKit::EventPtr> events;

    auto array = objarray.as< QList<msgpack::object> >();
    for (auto& evtObj : array) {
        auto event = eventFromMsgpackObject(evtObj);
        events.append(RBKit::EventPtr(event));
    }

    return events;
}


RBKit::EventParser::EventParser(const QByteArray& message)
    : rawMessage(message)
{
    msgpack::unpack(&unpacked, message.data(), message.size());
}


RBKit::EventDataBase* RBKit::EventParser::parseEvent() const
{
    // by default event will be of type collection
    auto eventType = guessEvent(unpacked.get());
    Q_ASSERT(RBKit::EtEventCollection == eventType);

    auto map = unpacked.get().as< QMap<unsigned int, msgpack::object> >();
    auto timestamp = map[RBKit::EfTimestamp].as<double>();
    auto ts = QDateTime::fromMSecsSinceEpoch(timestamp);

    auto events = parseEvents(map[RBKit::EfPayload]);
    return new RBKit::EvtCollection(ts, eventType, events);
}


RBKit::EventType
RBKit::EventParser::guessEvent(const msgpack::object& object) const
{
    auto map = object.as< QMap<unsigned int, msgpack::object> >();
    auto eventType = map[RBKit::EfEventType].as<unsigned int>();
    return static_cast<RBKit::EventType>(eventType);
}


QList<RBKit::ObjectDetailPtr>
RBKit::EventParser::parseObjects(const msgpack::object& object) const
{
    QList<RBKit::ObjectDetailPtr> objects;

    msgpack::object_array list = object.via.array;
    for (uint32_t iter = 0; iter != list.size; ++iter) {
        objects << list.ptr[iter].as< RBKit::ObjectDetailPtr >();
    }

    qDebug() << objects.size();
    return objects;
}


// ============================== different events ==============================

RBKit::EventDataBase::EventDataBase(QDateTime ts, RBKit::EventType eventType)
    : timestamp(ts), eventType(eventType)
{
}


RBKit::EvtNewObject::EvtNewObject(QDateTime ts, RBKit::EventType eventType,
                                  RBKit::ObjectDetailPtr _object)
    : EventDataBase(ts, eventType)
    , object(_object)
{

}

void RBKit::EvtNewObject::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}

RBKit::EvtDelObject::EvtDelObject(QDateTime ts, RBKit::EventType eventType,
                                  quint64 objectId_)
    : EventDataBase(ts, eventType)
    , objectId(objectId_)
{ }

void RBKit::EvtDelObject::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtGcStats::EvtGcStats(QDateTime ts, RBKit::EventType eventType,
                              QVariantMap _payload)
    : EventDataBase(ts, eventType)
    , payload(_payload)
{ }

void RBKit::EvtGcStats::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtGcStart::EvtGcStart(QDateTime ts, RBKit::EventType eventType)
    : EventDataBase(ts, eventType)
{}

void RBKit::EvtGcStart::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtGcStartM::EvtGcStartM(QDateTime ts, RBKit::EventType eventType)
    : EventDataBase(ts, eventType)
{}

void RBKit::EvtGcStartM::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtGcStop::EvtGcStop(QDateTime ts, RBKit::EventType eventType)
    : EventDataBase(ts, eventType)
{}

void RBKit::EvtGcStop::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtObjectDump::EvtObjectDump(QDateTime ts, RBKit::EventType eventType,
                                    QList<RBKit::ObjectDetailPtr> _objects)
    : EventDataBase(ts, eventType)
    , objects(_objects)
{ }

void RBKit::EvtObjectDump::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}

RBKit::EvtCollection::EvtCollection(QDateTime ts, RBKit::EventType eventType,
                                    QList<RBKit::EventPtr> _events)
    : EventDataBase(ts, eventType)
    , events(_events)
{}

void RBKit::EvtCollection::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}
