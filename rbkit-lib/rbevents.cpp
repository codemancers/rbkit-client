#include <msgpack.hpp>
#include "subscriber.h"
#include "rbevents.h"

#include <QDebug>


static QVariantList parseMsgpackObjectArray(msgpack::object_array);
static QVariantMap parseMsgpackObjectMap(msgpack::object_map);


static QVariant parseMsgpackObject(msgpack::object obj)
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
static QVariantMap parseMsgpackObjectMap(msgpack::object_map obj)
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


static QVariantList parseMsgpackObjectArray(msgpack::object_array array)
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
    // qDebug() << map << map["payload"];
    if(map["event_type"] != "obj_created" && map["event_type"] != "obj_destroyed")
        qDebug() << "Received event of type : " << map["event_type"].toString();

    QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(map["timestamp"].toULongLong());
    if (map["event_type"] == "obj_created") {
        return new RBKit::EvtNewObject(timestamp, map["event_type"].toString(), map["payload"].toMap());
    } else if (map["event_type"] == "obj_destroyed") {
        return new RBKit::EvtDelObject(timestamp, map["event_type"].toString(), map["payload"].toMap());
    } else if (map["event_type"] == "gc_stats") {
        return new RBKit::EvtGcStats(timestamp, map["event_type"].toString(), map["payload"].toMap());
    } else if (map["event_type"] == "gc_start") {
        return new RBKit::EvtGcStart(timestamp, map["event_type"].toString());
    } else if (map["event_type"] == "gc_end_s") {
        return new RBKit::EvtGcStop(timestamp, map["event_type"].toString());
    } else if (map["event_type"] == "object_space_dump") {
        return new RBKit::EvtObjectDump(timestamp, map["event_type"].toString(), map["payload"].toList());
    } else {
        qDebug() << "Unable to parse event of type" << map["event_type"];
        return NULL;
    }
}



RBKit::EventDataBase::EventDataBase(QDateTime ts, QString eventName)
    : timestamp(ts), eventName(eventName)
{
}


RBKit::EvtNewObject::EvtNewObject(QDateTime ts, QString eventName, QVariantMap payload)
    : EventDataBase(ts, eventName)
    , objectId(StringUtil::hextoInt(payload["object_id"].toString()))
    , className(payload["class"].toString())
{

}

void RBKit::EvtNewObject::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}

RBKit::EvtDelObject::EvtDelObject(QDateTime ts, QString eventName, QVariantMap payload)
    : EventDataBase(ts, eventName)
    , objectId(StringUtil::hextoInt(payload["object_id"].toString()))
{ }

void RBKit::EvtDelObject::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtGcStats::EvtGcStats(QDateTime ts, QString eventName, QVariantMap _payload)
    : EventDataBase(ts, eventName)
    , payload(_payload)
{ }

void RBKit::EvtGcStats::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtGcStart::EvtGcStart(QDateTime ts, QString eventName)
    : EventDataBase(ts, eventName)
{}

void RBKit::EvtGcStart::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtGcStop::EvtGcStop(QDateTime ts, QString eventName)
    : EventDataBase(ts, eventName)
{}

void RBKit::EvtGcStop::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtObjectDump::EvtObjectDump(QDateTime ts, QString eventName, QVariantList _payload)
    : EventDataBase(ts, eventName)
    , payload(_payload)
{ }

void RBKit::EvtObjectDump::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}
