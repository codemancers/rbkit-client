#include "testrbevents.h"
#include "rbevents.h"
#include <QDebug>

using namespace RBKit;

static QByteArray msgpackDataFromFile(const QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    return file.readAll();
}


// test whether parser is parsing create event properly or not.
void TestRbEvents::testParseObjectCreateEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/objcreated");
    RBKit::EventParser eventParser(data);

    EventDataBase* base = eventParser.parseEvent();
    QVERIFY(base);

    EvtCollection* collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtNewObject* event = dynamic_cast<EvtNewObject*>(collection->events[0].data());
    QVERIFY(event);
    QVERIFY(0x10190ed70 == event->object->objectId);
}

// test whether parser is parsing destroy event properly or not.
void TestRbEvents::testParseObjectDestroyEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/objdestroyed");
    RBKit::EventParser eventParser(data);

    EventDataBase* base = eventParser.parseEvent();
    QVERIFY(base);

    EvtCollection* collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtDelObject* event = dynamic_cast<EvtDelObject*>(collection->events[0].data());
    QVERIFY(event);
}

// test whether parser is parsing gc stats event properly or not.
void TestRbEvents::testParseGcStatsEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/gcstats");
    RBKit::EventParser eventParser(data);

    EventDataBase* base = eventParser.parseEvent();
    QVERIFY(base);

    EvtCollection* collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtGcStats* event = dynamic_cast<EvtGcStats*>(collection->events[0].data());
    QVERIFY(event);
}

void TestRbEvents::testParseGCStartEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/gc_start");
    RBKit::EventParser eventParser(data);

    EventDataBase *base = eventParser.parseEvent();
    QVERIFY(base);

    EvtCollection* collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtGcStart* event = dynamic_cast<EvtGcStart*>(collection->events[0].data());
    QVERIFY(event);

    QVERIFY(event->eventName == "gc_start");
}


void TestRbEvents::testParseObjectDumpEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/objectdump");
    RBKit::EventParser eventParser(data);

    EventDataBase *base = eventParser.parseEvent();
    QVERIFY(base);

    EvtCollection* collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtObjectDump* event = dynamic_cast<EvtObjectDump*>(collection->events[0].data());
    QVERIFY(event);
}
