#ifndef RBKIT_SQLCONNECTIONPOOL_H
#define RBKIT_SQLCONNECTIONPOOL_H

#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QSqlError>

#include "objectstore.h"

namespace RBKit {

class SqlConnectionPool
{
    SqlConnectionPool();
    static SqlConnectionPool *singleton;
    int currentVersion;
    QSqlDatabase database;
    QSqlQuery query;
    std::vector<int> snapshots;
public:
    static SqlConnectionPool* getInstance();
    void setupDatabase();
    void prepareTables();
    void loadSnapshot(ObjectStore *objectStore);
    void addObject(int object_id, const QString& className, int size, int referenceCount, const QString& file, int line);
    void addReference(int parentId, int childId);
};

} // namespace RBKit

#endif // RBKIT_SQLCONNECTIONPOOL_H
