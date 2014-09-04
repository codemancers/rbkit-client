#ifndef RBKIT_SQLCONNECTIONPOOL_H
#define RBKIT_SQLCONNECTIONPOOL_H

#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QSqlError>

#include "objectstore.h"
#include "heapitem.h"

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
    HeapItem *rootOfSnapshot(int snapShotVersion);

    int getCurrentVersion();
};

} // namespace RBKit

#endif // RBKIT_SQLCONNECTIONPOOL_H
