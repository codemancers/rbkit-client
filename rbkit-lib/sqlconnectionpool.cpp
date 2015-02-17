#include "sqlconnectionpool.h"
#include "model/appstate.h"
#include "model/heap_item_types/heapitem.h"
#include "rbeventparser.h"

namespace RBKit {

SqlConnectionPool* SqlConnectionPool::singleton = NULL;

SqlConnectionPool::SqlConnectionPool()
    : currentVersion(0)
{
    setupDatabase();
}

SqlConnectionPool *SqlConnectionPool::getInstance() {
    if (!singleton) {
        singleton = new SqlConnectionPool();
        return singleton;
    } else {
        return singleton;
    }
}

void SqlConnectionPool::setupDatabase()
{
    QFile file("/tmp/rbkit.db");
    if (file.exists()) {
        file.remove();
    }
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("/tmp/rbkit.db");
    currentVersion = 0;

    if (!database.open()) {
        qDebug() << query.lastError();
        return;
    }
    query = QSqlQuery(database);
    qDebug() << "Setting up database done";
}

void SqlConnectionPool::closeDatabase()
{
    database.close();
    database.removeDatabase("/tmp/rbkit.db");
}

void SqlConnectionPool::prepareTables()
{
    QVector<QString> objectCreation;
    objectCreation.append(QString("create table rbkit_objects_%0(id integer primary key"
                                  ", class_name varchar, size integer "
                                  ", reference_count integer, file varchar)").arg(currentVersion));
    objectCreation.append(QString("create table rbkit_object_references_%0(id integer primary key asc"
                                  ", object_id integer"
                                  ", child_id integer)").arg(currentVersion));
//    objectCreation.append(QString("create index index_on_classname on rbkit_objects_%0 (class_name)").arg(currentVersion));
//    objectCreation.append(QString("create index index_on_child_id on rbkit_object_references_%0 (child_id)").arg(currentVersion));

    for(auto createStatement : objectCreation) {
        if (!query.exec(createStatement)) {
            qDebug() << "Error creating table " << createStatement;
            qDebug() << query.lastError();
        }
    }
    qDebug() << "Preparing tables done";
}

void SqlConnectionPool::loadSnapshot(ObjectStore *objectStore)
{
    currentVersion += 1;
    prepareTables();

    qDebug() << "Loading db snapshot";
    qDebug() << QSqlDatabase::database().transaction();

    if (!query.prepare(
                QString("insert into rbkit_objects_%0(id, class_name, size, reference_count, file) values (?, ?, ?, ?, ?)")
                .arg(currentVersion))) {
        qDebug() << query.lastError();
        return;
    }

    objectStore->insertObjectsInDB(query);
    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 50);

    if (!query.prepare(QString("insert into rbkit_object_references_%0(object_id, child_id) values (?, ?)").arg(currentVersion)))
        qDebug() << query.lastError();

    objectStore->insertReferences(query);

    qDebug() << QSqlDatabase::database().commit();

    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 80);
}

void SqlConnectionPool::loadSnapshot2(const QByteArray rawMessage)
{
    currentVersion += 1;
    prepareTables();

    const RBKit::EventParser eventParser(*iter);
    auto dump = eventParser->rawObjectDump();

    qDebug() << "Loading db snapshot";
    qDebug() << QSqlDatabase::database().transaction();

    QSharedPointer<char> buffer;
    buffer.reset( (char*)malloc(102400) );

    msgpack::object_array list = object.via.array;
    for (uint32_t iter = 0; iter != list.size; ++iter) {
        objects << list.ptr[iter].as< RBKit::ObjectDetailPtr >();
    }

    objectStore->insertObjectsInDB(query);
    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 50);

    if (!query.prepare(QString("insert into rbkit_object_references_%0(object_id, child_id) values (?, ?)").arg(currentVersion)))
        qDebug() << query.lastError();

    objectStore->insertReferences(query);

    qDebug() << QSqlDatabase::database().commit();

    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 80);
}

HeapItem *SqlConnectionPool::rootOfSnapshot(int snapShotVersion)
{
    HeapItem *rootItem = new HeapItem(snapShotVersion);
    QSqlQuery searchQuery(QString("select class_name, count(id) as object_count, "
                          "sum(reference_count) as total_ref_count, sum(size) as total_size from rbkit_objects_%0 group by (class_name)").arg(snapShotVersion));
    while(searchQuery.next()) {
        HeapItem* item = new HeapItem(searchQuery.value(0).toString(), searchQuery.value(1).toInt(),
                                            searchQuery.value(2).toInt(), searchQuery.value(3).toInt(), snapShotVersion);
        rootItem->addChildren(item);
    }
    rootItem->childrenFetched = true;
    rootItem->setIsSnapshot(true);
    rootItem->computePercentage();
    return rootItem;
}

int SqlConnectionPool::getCurrentVersion()
{
   return currentVersion;
}

} // namespace RBKit
