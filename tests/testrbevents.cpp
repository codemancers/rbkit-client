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
    EventDataBase* base = parseEvent(data);
    QVERIFY(base);

    EvtNewObject* event = dynamic_cast<EvtNewObject*>(base);
    QVERIFY(event);
    QVERIFY(0x7f879309d090 == event->object->objectId);
}

// test whether parser is parsing destroy event properly or not.
void TestRbEvents::testParseObjectDestroyEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/objdestroyed");
    EventDataBase* base = parseEvent(data);
    QVERIFY(base);

    EvtDelObject* event = dynamic_cast<EvtDelObject*>(base);
    QVERIFY(event);
    QVERIFY(0x7f879309ff98 == event->objectId);
}

// test whether parser is parsing gc stats event properly or not.
void TestRbEvents::testParseGcStatsEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/gcstats");
    EventDataBase* base = parseEvent(data);
    QVERIFY(base);

    EvtGcStats* event = dynamic_cast<EvtGcStats*>(base);
    QVERIFY(event);
}

void TestRbEvents::testParseGCStartEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/gc_start");
    EventDataBase *base = parseEvent(data);

    QVERIFY(base);

    EvtGcStart *event = dynamic_cast<EvtGcStart *>(base);
    QVERIFY(event);

    QVERIFY(RBKit::EtGcStart == event->eventType);
}


void TestRbEvents::testParseObjectDumpEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/objectdump");
    EventDataBase *base = parseEvent(data);

    QVERIFY(base);

    EvtObjectDump *event = dynamic_cast<EvtObjectDump *>(base);
    QVERIFY(event);
}
