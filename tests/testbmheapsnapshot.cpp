#include "testbmheapsnapshot.h"
#include "rbeventparser.h"
#include "model/objectstore.h"
#include "sqlconnectionpool.h"
#include <QDebug>
#include "sqlite3.h"

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
    sqlite3* db(NULL);
    sqlite3_stmt* objStmt(NULL);
    sqlite3_stmt* refStmt(NULL);

    auto filename = (QString("/tmp/bm") + QTime::currentTime().toString())
        .toStdString().c_str();
    qDebug() << sqlite3_open(filename, &db);

    auto objectsTable = "create table rbkit_objects(id integer primary key"
        ", class_name varchar, size integer "
        ", reference_count integer, file varchar)";
    qDebug() << sqlite3_exec(db, objectsTable, NULL, NULL, NULL);

    auto refsTable = "create table rbkit_object_references(id integer primary key asc"
        ", object_id integer"
        ", child_id integer)";
    qDebug() << sqlite3_exec(db, refsTable, NULL, NULL, NULL);

    const char* tail(NULL);
    qDebug() << sqlite3_prepare_v2(db,
                                   "INSERT INTO rbkit_objects VALUES (?, ?, ?, ?, ?)",
                                   500, &objStmt, &tail);
    qDebug() << sqlite3_prepare_v2(db,
                                   "INSERT INTO rbkit_object_references(object_id, child_id) VALUES (?, ?);",
                                   500, &refStmt, &tail);

    QBENCHMARK_ONCE {
        // RBKit::SqlConnectionPool::getInstance()->loadSnapshot(store.data());

        char* error(NULL);
        qDebug() << sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &error);
        qDebug() << error;

        for (auto& object : store->objectStore) {
            sqlite3_bind_int64(objStmt, 1, object->objectId);
            sqlite3_bind_text(objStmt,  2, object->className.toStdString().c_str(),
                              object->className.size(), SQLITE_TRANSIENT);
            sqlite3_bind_int64(objStmt, 3, object->size);
            sqlite3_bind_int64(objStmt, 4, object->references.size());
            sqlite3_bind_text(objStmt,  5, object->getFileLine().toStdString().c_str(),
                              object->getFileLine().size(), SQLITE_TRANSIENT);

            sqlite3_step(objStmt);
            sqlite3_clear_bindings(objStmt);
            sqlite3_reset(objStmt);

            for (auto& ref : object->references) {
                sqlite3_bind_int64(refStmt, 1, object->objectId);
                sqlite3_bind_int64(refStmt, 2, ref);

                sqlite3_step(refStmt);
                sqlite3_clear_bindings(refStmt);
                sqlite3_reset(refStmt);
            }
        }

        qDebug() << sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &error);
        qDebug() << error;

        // iterating through objects, and references takes around 400ms, so not
        // a big deal.
        // unsigned long count = 0, refCount = 0;
        // for (auto& object : store->objectStore) {
        //     for (auto& ref : object->references) {
        //         ++refCount;
        //     }

        //     ++count;
        // }

        // qDebug() << count << refCount;
    }

    sqlite3_close(db);
}
