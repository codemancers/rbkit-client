#include "testrbevents.h"
#include "rbeventparser.h"
#include <QDebug>

using namespace RBKit;


DECLARE_TEST(TestRbEvents)


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

    auto base = eventParser.parseEvent();
    QVERIFY(base);

    auto collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtNewObject* event = dynamic_cast<EvtNewObject*>(collection->events[0].data());
    QVERIFY(event);
    QVERIFY(140220264206480 == event->object->objectId);
}

// test whether parser is parsing destroy event properly or not.
void TestRbEvents::testParseObjectDestroyEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/objdestroyed");
    RBKit::EventParser eventParser(data);

    auto base = eventParser.parseEvent();
    QVERIFY(base);

    auto collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtDelObject* event = dynamic_cast<EvtDelObject*>(collection->events[0].data());
    QVERIFY(event);
    QVERIFY(0x7f879309ff98 == event->objectId);
}

// test whether parser is parsing gc stats event properly or not.
void TestRbEvents::testParseGcStatsEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/gcstats");
    RBKit::EventParser eventParser(data);

    auto base = eventParser.parseEvent();
    QVERIFY(base);

    auto collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtGcStats* event = dynamic_cast<EvtGcStats*>(collection->events[0].data());
    QVERIFY(event);
}

void TestRbEvents::testParseGCStartEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/gc_start");
    RBKit::EventParser eventParser(data);

    auto base = eventParser.parseEvent();
    QVERIFY(base);

    auto collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtGcStart* event = dynamic_cast<EvtGcStart*>(collection->events[0].data());
    QVERIFY(event);

    QVERIFY(RBKit::EtGcStart == event->eventType);
}


void TestRbEvents::testParseObjectDumpEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/split_dump");
    RBKit::EventParser eventParser(data);

    auto base = eventParser.parseEvent();
    QVERIFY(base);

    auto collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtObjectDump* event = dynamic_cast<EvtObjectDump*>(collection->events[0].data());
    QVERIFY(event);
}


void TestRbEvents::testParseCpuSampleEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/cpusample");
    RBKit::EventParser eventParser(data);

    auto base = eventParser.parseEvent();
    QVERIFY(base);

    auto collection = dynamic_cast<EvtCollection*>(base);
    QVERIFY(collection);

    EvtCpuSample* event = dynamic_cast<EvtCpuSample*>(collection->events[0].data());
    QVERIFY(event);
}
