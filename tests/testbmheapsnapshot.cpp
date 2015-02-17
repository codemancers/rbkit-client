#include "testbmheapsnapshot.h"
#include "rbeventparser.h"
#include "model/objectstore.h"
#include "sqlconnectionpool.h"
#include <QDebug>
#include "sqlite3.h"
#include <cstdio>

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

    const char* insertStmt = "INSERT INTO rbkit_objects SELECT %ld AS id,"
        "'%d' AS class_name,%d AS size,%d AS reference_count, '%d' AS file ";
    const char* unionStmt  = "UNION SELECT %ld,'%d',%d,%d,'%d'";

    const char* refInsertStmt = "INSERT INTO rbkit_object_references"
        " SELECT %ld AS id,%ld AS object_id,%ld AS child_id";
    const char* refUnionStmt  = " UNION SELECT %ld,%ld,%ld";

    QSharedPointer<char> buffer, refBuffer;
    refBuffer.reset( (char*)malloc(102400) );
    buffer.reset( (char*)malloc(102400) );

    QBENCHMARK_ONCE {
        // RBKit::SqlConnectionPool::getInstance()->loadSnapshot(store.data());

        char* error(NULL);
        qDebug() << sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &error);
        qDebug() << error;

        quint64 objectCount = 0, referenceCount = 0, refId = 1;
        auto refIter = refBuffer.data();
        auto iter = buffer.data();
        quint64 sprintfLength = 0;

        for (auto& object : store->objectStore) {
            if (objectCount == 500) {
                *iter = '\0';

                sqlite3_exec(db, buffer.data(), NULL, NULL, &error);
                iter = buffer.data();
                objectCount = 0;
            }

            if (!objectCount) {
                sprintfLength =
                    sprintf(iter, insertStmt, object->objectId,
                            object->objectId,
                            object->size, object->references.size(),
                            object->objectId);
            } else {
                sprintfLength =
                    sprintf(iter, unionStmt, object->objectId,
                            object->objectId,
                            object->size, object->references.size(),
                            object->objectId);
            }

            iter += sprintfLength;
            ++objectCount;

            for (auto& ref : object->references) {
                if (referenceCount == 500) {
                    *refIter = '\0';

                    sqlite3_exec(db, refBuffer.data(), NULL, NULL, &error);
                    refIter = refBuffer.data();
                    referenceCount = 0;
                }

                if (!referenceCount) {
                    sprintfLength =
                        sprintf(refIter, refInsertStmt, refId, object->objectId, ref);
                } else {
                    sprintfLength =
                        sprintf(refIter, refUnionStmt, refId, object->objectId, ref);
                }

                refIter += sprintfLength;
                ++referenceCount;
                ++refId;
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
