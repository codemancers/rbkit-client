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
class RbDumpParser;


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
    void beginTransaction();
    void beginObjectInjection();
    void beginReferenceInjection();
    void commitTransaction();
    void persistObject(const ObjectDetail& object);
    void loadSnapshot(ObjectStore *objectStore);

    void persistObjects(RbDumpParser& parser);
    void persistReferences(const QHash< quint64, QList<quint64> >&);

    HeapItem *rootOfSnapshot(int snapShotVersion);

    int getCurrentVersion();
};

} // namespace RBKit

#endif // RBKIT_SQLCONNECTIONPOOL_H
