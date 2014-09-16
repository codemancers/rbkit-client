#include "testobjectdump.h"
#include "rbevents.h"
#include "jsbridge.h"
#include "subscriber.h"
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
    auto data = msgpackDataFromFile(":/tests/msgpack/hugedump");
    qDebug() << "benchmark data size" << data.size();

    EventDataBase* base = NULL;
    QBENCHMARK {
        base = parseEvent(data);
    }

    QVERIFY(base);

    EvtObjectDump* event = dynamic_cast<EvtObjectDump *>(base);
    QVERIFY(event);

    qDebug() << "total objects :" << event->objects.size();
}


void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsEmpty()
{
    auto data = msgpackDataFromFile(":/tests/msgpack/hugedump");

    auto base = parseEvent(data);
    EvtObjectDump* evt = dynamic_cast<EvtObjectDump *>(base);
    qDebug() << "total objects :" << evt->objects.size();

    // Create a subscriber
    auto jsBridge = new RBKit::JsBridge();
    auto subscriber = new Subscriber(jsBridge);

    qDebug() << "populating object store for first time";
    QBENCHMARK {
        subscriber->processEvent(*evt);
    }
}

void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsFull()
{
    auto data = msgpackDataFromFile(":/tests/msgpack/hugedump");

    auto base = parseEvent(data);
    EvtObjectDump* evt = dynamic_cast<EvtObjectDump *>(base);
    qDebug() << "total objects :" << evt->objects.size();

    // Create a subscriber
    auto jsBridge = new RBKit::JsBridge();
    auto subscriber = new Subscriber(jsBridge);

    subscriber->processEvent(*evt);

    qDebug() << "populating object store again";
    QBENCHMARK {
        subscriber->processEvent(*evt);
    }
}
