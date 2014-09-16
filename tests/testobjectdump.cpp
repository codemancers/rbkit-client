#include "testobjectdump.h"
#include "rbevents.h"
#include <QDebug>

using namespace RBKit;

static QByteArray msgpackDataFromFile(const QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    return file.readAll();
}


void TestObjectDump::testBenchmarkParseObjectDump()
{
    QByteArray data = msgpackDataFromFile(":/tests/msgpack/hugedump");
    qDebug() << "benchmark data size" << data.size();

    EventDataBase* base = NULL;
    QBENCHMARK {
        base = parseEvent(data);
    }

    QVERIFY(base);

    EvtObjectDump *event = dynamic_cast<EvtObjectDump *>(base);
    QVERIFY(event);

    qDebug() << "total objects :" << event->objects.size();
}
