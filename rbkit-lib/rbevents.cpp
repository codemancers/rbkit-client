#include <msgpack.hpp>
#include "subscriber.h"
#include "rbevents.h"
#include "mpparser.h"

#include "debug.h"


static RBKit::EventDataBase* makeEventFromUnpacked(msgpack::unpacked& unpacked);
static QList<RBKit::EventPtr> parseEventsFromMsg(const msgpack::object&);


static QString guessEvent(msgpack::object& object)
{
    auto map = object.as< QMap<QString, msgpack::object> >();
    return map["event_type"].as<QString>();
}

static RBKit::EventDataBase*
makeEventFromObject(msgpack::object& object)
{
    // first try to guess the event type.
    auto map = object.as< QMap<QString, msgpack::object> >();

    auto eventType = guessEvent(object);
    auto timestamp = QDateTime::fromMSecsSinceEpoch(map["timestamp"].as<double>());
    auto payload = map["payload"];

    if (eventType == "obj_created") {
        auto object = payload.as<RBKit::ObjectDetailPtr>();
        return new RBKit::EvtNewObject(timestamp, eventType, object);
    } else if (eventType == "obj_destroyed") {
        return new RBKit::EvtDelObject(timestamp, eventType, payload.as<QVariantMap>());
    } else if (eventType == "gc_stats") {
        return new RBKit::EvtGcStats(timestamp, eventType, payload.as<QVariantMap>());
    } else if (eventType == "gc_start") {
        return new RBKit::EvtGcStart(timestamp, eventType);
    } else if (eventType == "gc_end_s") {
        return new RBKit::EvtGcStop(timestamp, eventType);
    } else if (eventType == "event_collection") {
        auto events = parseEventsFromMsg(payload);
        return new RBKit::EvtCollection(timestamp, eventType, events);
    } else {
        qDebug() << "Unable to parse event of type" << eventType;
        return NULL;
    }
}

static RBKit::EventDataBase*
makeEventFromUnpacked(msgpack::unpacked& unpacked)
{
    auto object = unpacked.get();
    auto type = guessEvent(object);
    INFO1("event-type: %s", type.toStdString().c_str());

    if (type == "object_space_dump") {
        // special handling for object dump.
        auto map = object.as< QMap<QString, msgpack::object> >();
        auto ts = QDateTime::fromMSecsSinceEpoch(map["timestamp"].as<double>());
        return new RBKit::EvtObjectDump(ts, type, unpacked);
    } else {
        return makeEventFromObject(object);
    }
}


static QList<RBKit::EventPtr>
parseEventsFromMsg(const msgpack::object& objarray)
{
    QList<RBKit::EventPtr> events;

    auto array = objarray.as< QList<msgpack::object> >();
    for (auto& evtObj : array) {
        auto event = makeEventFromObject(evtObj);
        events.append(RBKit::EventPtr(event));
    }

    return events;
}


RBKit::EventDataBase*
RBKit::parseEvent(const QByteArray& message)
{
    msgpack::unpacked unpacked;
    msgpack::unpack(&unpacked, message.data(), message.size());

    return makeEventFromUnpacked(unpacked);
}


// ============================== different events ==============================

RBKit::EventDataBase::EventDataBase(QDateTime ts, QString eventName)
    : timestamp(ts), eventName(eventName)
{
}


RBKit::EvtNewObject::EvtNewObject(QDateTime ts, QString eventName,
                                  RBKit::ObjectDetailPtr _object)
    : EventDataBase(ts, eventName)
    , object(_object)
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


RBKit::EvtObjectDump::EvtObjectDump(QDateTime ts, QString eventName,
                                    msgpack::unpacked heapDump)
    : EventDataBase(ts, eventName)
    , dump(heapDump)
{ }

void RBKit::EvtObjectDump::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}

RBKit::EvtCollection::EvtCollection(QDateTime ts, QString eventName,
                                    QList<RBKit::EventPtr> _events)
    : EventDataBase(ts, eventName)
    , events(_events)
{}

void RBKit::EvtCollection::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}
