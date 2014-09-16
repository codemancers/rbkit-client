#include "testobjectdump.h"
#include "rbevents.h"
#include "objectstore.h"
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

    // Create an objectstore
    ObjectStore store;

    qDebug() << "populating object store for first time";
    QBENCHMARK {
        store.updateFromSnapshot(evt->objects);
    }
}

void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsFull()
{
    auto data = msgpackDataFromFile(":/tests/msgpack/hugedump");

    auto base = parseEvent(data);
    EvtObjectDump* evt = dynamic_cast<EvtObjectDump *>(base);
    qDebug() << "total objects :" << evt->objects.size();

    // Create an objectstore
    ObjectStore store;

    store.updateFromSnapshot(evt->objects);

    qDebug() << "populating object store again";
    QBENCHMARK {
        store.updateFromSnapshot(evt->objects);
    }
}
