#include "testrbevents.h"
#include "rbevents.h"
#include <QDebug>

using namespace RBKit;

void TestRbEvents::testParseObjectCreateEvent()
{
    QString dump(":/tests/objcreated.msgpack");
    QFile file(dump);

    QVERIFY(file.open(QFile::ReadOnly));

    QByteArray data = file.readAll();
    qDebug() << data.size();

    EventDataBase* event = parseEvent(data);
    QVERIFY(event);

    EvtNewObject* evt = dynamic_cast<EvtNewObject*>(event);
    QVERIFY(evt);
}

void TestRbEvents::testParseObjectDestroyEvent()
{
}
