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
    QByteArray data = msgpackDataFromFile(":/tests/objcreated.msgpack");
    EventDataBase* base = parseEvent(data);
    QVERIFY(base);

    EvtNewObject* event = dynamic_cast<EvtNewObject*>(base);
    QVERIFY(event);
    QVERIFY(0x7f879309d090 == event->objectId);
}

// test whether parser is parsing destroy event properly or not.
void TestRbEvents::testParseObjectDestroyEvent()
{
    QByteArray data = msgpackDataFromFile(":/tests/objdestroyed.msgpack");
    EventDataBase* base = parseEvent(data);
    QVERIFY(base);

    EvtDelObject* event = dynamic_cast<EvtDelObject*>(base);
    QVERIFY(event);
}
