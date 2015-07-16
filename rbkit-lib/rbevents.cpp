#include "subscriber.h"
#include "rbevents.h"
#include "cpu/cpuprof.h"


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


RBKit::EvtObjectDump::EvtObjectDump(QDateTime ts, RBKit::EventType eventType, int correlationId,
                                    int completeObjectCount,
                                    QList<RBKit::ObjectDetailPtr> _objects)
    : EventDataBase(ts, eventType)
    , correlationId(correlationId), completeObjectCount(completeObjectCount)
    , objects(_objects)
{
    objectCount = objects.size();
}

void RBKit::EvtObjectDump::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}

RBKit::EvtCollection::EvtCollection(QDateTime ts, RBKit::EventType eventType,
                                    QList<RBKit::EventPtr> _events,
                                    quint64 _counter)
    : EventDataBase(ts, eventType)
    , events(_events)
    , messageCounter(_counter)
{}

void RBKit::EvtCollection::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}


RBKit::EvtHandshake::EvtHandshake(QDateTime ts, RBKit::EventType eventType, QVariantMap payload)
    : EventDataBase(ts, eventType)
{
    processName = payload["process_name"].toString();
    pwd = payload["pwd"].toString();
    pid = payload["pid"].toUInt();
    tracingFlag = payload["object_trace_enabled"].toInt() == 0 ? false : true;
    rbkitServerVersion = payload["rbkit_server_version"].toString();
    rbkitProtocolVersion = payload["rbkit_protocol_version"].toString();
}

void RBKit::EvtHandshake::process(Subscriber &processor) const
{
    processor.processEvent(*this);
}

RBKit::EvtCpuSample::EvtCpuSample(QDateTime ts, RBKit::EventType eventType, QList<QMap<int, QVariant> > payload)
    : EventDataBase(ts, eventType)
{
    //qDebug() << payload;
    CpuProf::decodeMap(payload);

}

void RBKit::EvtCpuSample::process(Subscriber &processor) const
{
    processor.processEvent(*this);
}
