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
    void closeDatabase();

    void prepareTables();
    void beginTransaction();
    void beginObjectInsertion();
    void beginReferenceInsertion();
    void commitTransaction();

    void loadSnapshot(ObjectStore *objectStore);
    void persistObjects(RbDumpParser& parser);

    void persistReferences(const QHash< quint64, QList<quint64> >&);
    void persistObject(const ObjectDetail& object);

    HeapItem *rootOfSnapshot(int snapShotVersion);

    int getCurrentVersion();
};

} // namespace RBKit

#endif // RBKIT_SQLCONNECTIONPOOL_H
