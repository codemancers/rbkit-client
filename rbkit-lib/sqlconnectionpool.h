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

// forward declarations
struct SQlite3Wrapper;


class HeapItem;

class SqlConnectionPool
{
    SqlConnectionPool();
    static SqlConnectionPool *singleton;

    QSharedPointer<SQlite3Wrapper> sqlite3;
    int currentVersion;
    QSqlDatabase database;
    QSqlQuery query;
    std::vector<int> snapshots;
public:
    static SqlConnectionPool* getInstance();
    void setupDatabase();
    void closeDatabase();
    void prepareTables();
    void loadSnapshot(ObjectStore *objectStore);
    HeapItem *rootOfSnapshot(int snapShotVersion);

    int getCurrentVersion();
};

} // namespace RBKit

#endif // RBKIT_SQLCONNECTIONPOOL_H
