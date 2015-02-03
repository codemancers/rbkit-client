#include "testobjectdump.h"
#include "rbeventparser.h"
#include "model/objectstore.h"
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
    objectDump = msgpackDataFromFile(":/tests/msgpack/split_dump");
    RBKit::EventParser eventParser(objectDump);

    auto collection = dynamic_cast<EvtCollection*>(eventParser.parseEvent());
    auto evt = dynamic_cast<EvtObjectDump *>(collection->events[0].data());
    event.reset(evt);
}

void TestObjectDump::testBenchmarkParseObjectDump()
{
    EventDataBase* base = NULL;
    QBENCHMARK {
        RBKit::EventParser eventParser(objectDump);
        base = eventParser.parseEvent();
    }

    QVERIFY(base);

    auto collection = dynamic_cast<EvtCollection*>(base);
    auto event = dynamic_cast<EvtObjectDump *>(collection->events[0].data());
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
