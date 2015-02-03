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
    auto objectDump = msgpackDataFromFile("/Users/yuva/00013");
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

        quint64 objectCount = 0, referenceCount = 0;
        QString objectSql, referenceSql;

        for (auto& object : store->objectStore) {
            if (objectCount == 500) {
                sqlite3_exec(db, objectSql.toStdString().c_str(),
                             NULL, NULL, &error);
                objectSql.clear();
                objectCount = 0;
            }

            if (!objectCount) {
                objectSql
                    .append("INSERT INTO rbkit_objects SELECT ")
                    .append(QString::number(object->objectId)).append(" AS id,'")
                    .append(object->className).append("' AS class_name,")
                    .append(QString::number(object->size)).append(" AS size,")
                    .append(QString::number(object->references.size())).append(" AS reference_count,'")
                    .append(object->getFileLine()).append("' AS file ");
            } else {
                objectSql
                    .append("UNION SELECT ")
                    .append(QString::number(object->objectId)).append(",'")
                    .append(object->className).append("',")
                    .append(QString::number(object->size)).append(",")
                    .append(QString::number(object->references.size())).append(",'")
                    .append(object->getFileLine()).append("' ");
            }

            ++objectCount;

            // for (auto& ref : object->references) {
            //     sqlite3_bind_int64(refStmt, 1, object->objectId);
            //     sqlite3_bind_int64(refStmt, 2, ref);

            //     sqlite3_step(refStmt);
            //     sqlite3_clear_bindings(refStmt);
            //     sqlite3_reset(refStmt);
            // }
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
