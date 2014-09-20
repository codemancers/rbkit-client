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

void TestObjectDump::initTestCase()
{
    // read object dump, and parse it
    objectDump = msgpackDataFromFile(":/tests/msgpack/hugedump");

    auto evt = parseEvent(objectDump);
    event.reset(dynamic_cast<EvtObjectDump *>(evt));
}

void TestObjectDump::testBenchmarkParseObjectDump()
{
    EventDataBase* base = NULL;
    QBENCHMARK {
        base = parseEvent(objectDump);
    }

    QVERIFY(base);

    EvtObjectDump* event = dynamic_cast<EvtObjectDump *>(base);
    QVERIFY(event);

    qDebug() << "total objects :" << event->objects.size();
}


void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsEmpty()
{
    qDebug() << "total objects :" << event->objects.size();

    // Create an objectstore
    ObjectStore store;

    qDebug() << "populating object store for first time";
    QBENCHMARK {
        store.updateFromSnapshot(event->objects);
    }
}

void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsFull()
{
    qDebug() << "total objects :" << event->objects.size();

    // Create an objectstore
    ObjectStore store;

    store.updateFromSnapshot(event->objects);

    qDebug() << "populating object store again";
    QBENCHMARK {
        store.updateFromSnapshot(event->objects);
    }
}
