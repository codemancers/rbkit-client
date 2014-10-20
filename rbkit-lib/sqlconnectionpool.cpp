#include "sqlconnectionpool.h"
#include "model/appstate.h"
#include "model/heap_item_types/heapitem.h"
#include "mpparser.h"


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
    database.setDatabaseName(":memory:");

    if (!database.open()) {
        qDebug() << query.lastError();
        return;
    }
    query = QSqlQuery(database);
    qDebug() << "Setting up database done";
}

void SqlConnectionPool::prepareTables()
{
    currentVersion += 1;

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


void SqlConnectionPool::beginTransaction()
{
    if (!query.exec(QString("begin transaction"))) {
        qDebug() << query.lastError();
    }
}


void SqlConnectionPool::beginObjectInsertion()
{
    QString insertStatement("insert into rbkit_objects_%0"
                            "(id"
                            ", class_name"
                            ", size"
                            ", reference_count"
                            ", file)"
                            " values (?, ?, ?, ?, ?)");

    if ( !query.prepare(insertStatement.arg(currentVersion)) ) {
        qDebug() << query.lastError();
    }
}


void SqlConnectionPool::beginReferenceInsertion()
{
    QString insertStatement("insert into rbkit_object_references_%0"
                            "(object_id"
                            ", child_id)"
                            " values (?, ?)");
    if ( !query.prepare(insertStatement.arg(currentVersion)) ) {
        qDebug() << query.lastError();
    }
}


void SqlConnectionPool::commitTransaction()
{
    if ( !query.exec(QString("commit transaction")) ) {
        qDebug() << query.lastError();
    }

    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 80);

}

void SqlConnectionPool::loadSnapshot(ObjectStore *objectStore)
{
    static const unsigned int batchSize = 1000;
    QHash< quint64, QList<quint64> > references;
    references.reserve(1000);
    int commits(0);

    auto iter = objectStore->objectStore.begin();

    qDebug() << "Loading db snapshot";

    prepareTables();

    beginTransaction();
    beginObjectInsertion();
    for (unsigned int count = 0; iter != objectStore->objectStore.end();
         ++iter, ++count) {
        auto objectPtr = iter.value();
        references[objectPtr->objectId] = objectPtr->references;
        persistObject(*objectPtr);

        count += objectPtr->references.size();
        if (count >= batchSize) {
            beginReferenceInsertion();
            persistReferences(references);
            references.clear();

            commitTransaction();

            beginTransaction();
            beginObjectInsertion();

            count = 0;
            ++commits;
        }
    }
    commitTransaction();

    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 50);

    qDebug() << commits;
}



void SqlConnectionPool::persistObject(const RBKit::ObjectDetail& object)
{
    query.addBindValue(object.objectId);
    query.addBindValue(object.className);
    query.addBindValue(object.size);
    query.addBindValue(object.references.size());
    query.addBindValue(object.getFileLine());
    if (!query.exec()) {
        qDebug() << "id:" << object.objectId
                 << "error: %s" << query.lastError();
    }
}


void SqlConnectionPool::persistReferences(const QHash< quint64, QList<quint64> >& hash)
{
    for (auto iter = hash.begin(); iter != hash.end(); ++iter) {
        auto objectId = iter.key();
        auto refs = iter.value();

        for (auto ref : refs) {
            query.addBindValue(objectId);
            query.addBindValue(ref);
            if (!query.exec()) {
                qDebug() << "id:" << objectId
                         << "ref:" << ref
                         << "error: %s" << query.lastError();
            }
        }
    }
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
