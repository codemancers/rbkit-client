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
    sqlite3* db;
    sqlite3_stmt* stmt;

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

    QBENCHMARK_ONCE {
        // RBKit::SqlConnectionPool::getInstance()->loadSnapshot(store.data());

        char* error(NULL);
        qDebug() << sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &error);
        qDebug() << error;

        for (auto& object : store->objectStore) {
            auto objectSql = QString("INSERT INTO rbkit_objects VALUES (") +
                QString::number(object->objectId) + ", " +
                "'" + object->className + "', " +
                QString::number(object->size) + ", " +
                QString::number(object->references.size()) + ", " +
                "'" + object->getFileLine() + "');";

            char* sqlError(NULL);
            auto code = sqlite3_exec(db, objectSql.toStdString().c_str(), NULL,
                                     NULL, &sqlError);
            if (0 != code) {
                qDebug() << sqlError;
            }

            for (auto& ref : object->references) {
                auto refSql =
                    QString("INSERT INTO rbkit_object_references(object_id, child_id) VALUES (") +
                    QString::number(object->objectId) + ", " +
                    QString::number(ref) + ");";

                char* sqlError(NULL);
                auto code = sqlite3_exec(db, refSql.toStdString().c_str(), NULL,
                                         NULL, &sqlError);
                if (0 != code) {
                    qDebug() << sqlError;
                }
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
