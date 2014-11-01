#include "sqlconnectionpool.h"
#include "model/appstate.h"
#include "model/heap_item_types/heapitem.h"
#include "rbdumpworker.h"
#include "mpparser.h"
#include "debug.h"


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
        INFO1("error: %s", query.lastError().text().toStdString().c_str());
    }
}


void SqlConnectionPool::beginObjectInjection()
{
    QString insertStatement("insert into rbkit_objects_%0"
                            "(id"
                            ", class_name"
                            ", size"
                            ", reference_count"
                            ", file)"
                            " values (?, ?, ?, ?, ?)");

    if ( !query.prepare(insertStatement.arg(currentVersion)) ) {
        INFO1("error: %s", query.lastError().text().toStdString().c_str());
    }
}


void SqlConnectionPool::beginReferenceInjection()
{
    QString insertStatement("insert into rbkit_object_references_%0"
                            "(object_id"
                            ", child_id)"
                            " values (?, ?)");
    if ( !query.prepare(insertStatement.arg(currentVersion)) ) {
        INFO1("error: %s", query.lastError().text().toStdString().c_str());
    }
}


void SqlConnectionPool::commitTransaction()
{
    if ( !query.exec(QString("commit transaction")) ) {
        INFO1("error: %s", query.lastError().text().toStdString().c_str());
    }

    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 80);

}

void SqlConnectionPool::loadSnapshot(ObjectStore *objectStore)
{
    prepareTables();
    beginTransaction();

    qDebug() << "Loading db snapshot";
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

    commitTransaction();
}


void SqlConnectionPool::persistObjects(RBKit::RbDumpParser& parser)
{
    static const unsigned int batchSize = 100;
    QHash< quint64, QList<quint64> > references;
    references.reserve(1000);

    auto iter = parser.begin();

    prepareTables();

    beginTransaction();
    beginObjectInjection();
    for (unsigned int count = 0; iter != parser.end(); ++iter, ++count) {
        RBKit::ObjectDetail object;
        *iter >> object;

        references[object.objectId] = object.references;
        persistObject(object);

        count += object.references.size();
        if (count >= batchSize) {
            beginReferenceInjection();
            persistReferences(references);

            commitTransaction();
            beginTransaction();
            beginObjectInjection();

            count = 0;
            break;
        }
    }
    commitTransaction();
}


void SqlConnectionPool::persistObject(const RBKit::ObjectDetail& object)
{
    query.addBindValue(object.objectId);
    query.addBindValue(object.className);
    query.addBindValue(object.size);
    query.addBindValue(object.references.size());
    query.addBindValue(object.getFileLine());
    INFO1("id %llu", object.objectId);
    if (!query.exec()) {
        INFO1("error: %s", query.lastError().text().toStdString().c_str());
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
                INFO1("error: %s", query.lastError().text().toStdString().c_str());
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
