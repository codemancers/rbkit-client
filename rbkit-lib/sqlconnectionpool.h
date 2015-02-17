#ifndef RBKIT_SQLCONNECTIONPOOL_H
#define RBKIT_SQLCONNECTIONPOOL_H

#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QSqlError>

#include "model/objectstore.h"

namespace RBKit {

class HeapItem;

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
    void closeDatabase();
    void prepareTables();
    void loadSnapshot2(const QByteArray rawMessage);
    void loadSnapshot(ObjectStore *objectStore);
    HeapItem *rootOfSnapshot(int snapShotVersion);

    int getCurrentVersion();
};

} // namespace RBKit

#endif // RBKIT_SQLCONNECTIONPOOL_H
