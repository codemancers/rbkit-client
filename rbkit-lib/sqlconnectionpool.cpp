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

    if (!database.open())
        return database.lastError();

    query = QSqlQuery(database);

}

void SqlConnectionPool::prepareTables()
{
    QVector<QString> objectCreation;
    objectCreation.append(QString("create table rbkit_objects_%0(id integer primary key"
                                  ", class_name varchar, size integer "
                                  ", reference_count integer, file varchar"
                                  ", line integer)").arg(currentVersion));
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
                QString("insert into rbkit_objects_%0(id, class_name, size, reference_count, file, line) values (?, ?, ?, ?, ?, ?)")
                .arg(currentVersion))) {
        qDebug() << query.lastError();
        return;
    }


}

void SqlConnectionPool::addObject(int object_id, const QString &className, int size, int referenceCount, const QString &file, int line)
{

}

void SqlConnectionPool::addReference(int parentId, int childId)
{

}

} // namespace RBKit
