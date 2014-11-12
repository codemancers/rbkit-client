#include "testbmheapsnapshot.h"
#include "rbeventparser.h"
#include "model/objectstore.h"
#include "sqlconnectionpool.h"
#include <QDebug>

using namespace RBKit;

static QByteArray msgpackDataFromFile(const QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    return file.readAll();
}

void TestBmHeapSnapshot::initTestCase()
{
    // read object dump, and parse it
    auto objectDump = msgpackDataFromFile(":/tests/msgpack/hugedump");
    RBKit::EventParser eventParser(objectDump);

    auto collection = dynamic_cast<EvtCollection*>(eventParser.parseEvent());
    auto evt = dynamic_cast<EvtObjectDump *>(collection->events[0].data());
    QSharedPointer<RBKit::EvtObjectDump> event(evt);

    store.reset(new ObjectStore());
    store->updateFromSnapshot(event->objects);
}

void TestBmHeapSnapshot::testBenchmarkPersistingToDb()
{
    QBENCHMARK {
        RBKit::SqlConnectionPool::getInstance()->loadSnapshot(store.data());
    }
}
