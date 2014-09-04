#include "sqlconnectionpool.h"

#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace RBKit {

SqlConnectionPool* SqlConnectionPool::singleton = NULL;

SqlConnectionPool::SqlConnectionPool()
    : currentVersion(0)
{
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
    qDebug() << "Creating new shared resource";
    QFile file("/tmp/rbkit.db");
    if (file.exists()) {
        file.remove();
    }
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("/tmp/rbkit.db");

    if (!database.open()) {
        qDebug() << query.lastError();
        return
    }


    query = QSqlQuery(database);

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

    for(auto createStatement : objectCreation) {
        if (!query.exec(createStatement)) {
            qDebug() << "Error creating table " << createStatement;
            qDebug() << query.lastError();
        }
    }
}

void SqlConnectionPool::loadSnapshot(ObjectStore *objectStore)
{
    currentVersion += 1;
    prepareTables();

    if (!query.prepare(
                QString("insert into rbkit_objects_%0(id, class_name, size, reference_count, file) values (?, ?, ?, ?, ?)")
                .arg(currentVersion))) {
        qDebug() << query.lastError();
        return;
    }

    QHash<quint64, RBKit::ObjectDetail*>::iterator iter = objectStore->objectStore.constBegin();
    while(iter != objectStore->objectStore.constEnd()) {
        addObject(iter.value());
        ++iter;
    }
}

void SqlConnectionPool::addObject(ObjectDetail *objectDetail)
{
    query.addBindValue(objectDetail->objectId);
    query.addBindValue(objectDetail->className);
    query.addBindValue(objectDetail->size);
    query.addBindValue(objectDetail->references.size());
    query.addBindValue(objectDetail->getFileLine());
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

void SqlConnectionPool::addReference(int parentId, int childId)
{

}

} // namespace RBKit
