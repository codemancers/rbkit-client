#include <msgpack.hpp>
#include "subscriber.h"
#include "rbevents.h"
#include "mpparser.h"
#include "rbdebug.h"


RBKit::EventDataBase*
RBKit::EventParser::eventFromObject(msgpack::object& object) const
{
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
    } else if (eventType == "object_space_dump") {
        return new RBKit::EvtObjectDump(timestamp, eventType, rawMessage);
    } else {
        qDebug() << "Unable to parse event of type" << eventType;
        Q_ASSERT(false);

        return NULL;
    }
}


QList<RBKit::EventPtr>
RBKit::EventParser::parseEvents(const msgpack::object& objarray) const
{
    QList<RBKit::EventPtr> events;

    auto array = objarray.as< QList<msgpack::object> >();
    for (auto& evtObj : array) {
        auto event = eventFromObject(evtObj);
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
    auto type = guessEvent(unpacked.get());
    Q_ASSERT(type == "event_collection");

    auto map = unpacked.get().as< QMap<QString, msgpack::object> >();
    auto timestamp = map["timestamp"].as<double>();
    auto ts = QDateTime::fromMSecsSinceEpoch(timestamp);

    auto events = parseEvents(map["payload"]);
    return new RBKit::EvtCollection(ts, "event_collection", events);
}


QString RBKit::EventParser::guessEvent(const msgpack::object& object) const
{
    auto map = object.as< QMap<QString, msgpack::object> >();
    return map["event_type"].as<QString>();
}


msgpack::object RBKit::EventParser::extractObjectDump() const
{
    // by default event will be of type collection
    auto type = guessEvent(unpacked.get());
    Q_ASSERT(type == "event_collection");

    auto map = unpacked.get().as< QMap<QString, msgpack::object> >();
    auto events = map["payload"].as< QList<msgpack::object> >();

    for (auto& event : events) {
        if ("object_space_dump" == guessEvent(event)) {
            auto hash = event.as< QMap<QString, msgpack::object> >();
            return hash["payload"];
        }
    }

    Q_ASSERT(false);
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
                                    QByteArray message)
    : EventDataBase(ts, eventName)
    , rawMessage(message)
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
