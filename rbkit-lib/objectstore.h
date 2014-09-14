#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include <QHash>
#include <QVariant>
#include <QVariantMap>
#include <QMap>
#include <QMultiMap>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

class QSqlQuery;

#include "objectdetail.h"

namespace RBKit {
    class ObjectStore
    {
        class Sorter {
            const ObjectStore *objectStore;
        public:
            Sorter(const ObjectStore *_objectStore): objectStore(_objectStore) {};
            bool operator() (QString key1, QString key2) {
                return objectStore->objectTypeCount[key1] > objectStore->objectTypeCount[key2];
            }
        };

    public:
        // Store mapping between object-id and detail
        QHash<quint64, RBKit::ObjectDetailPtr> objectStore;
        // mapping between object class and its count
        QHash<QString, quint32> objectTypeCount;
        // Mapping between object class and the ids
        QMultiMap<QString, quint64> objectTypeIdMap;

        // database related functions
        void insertObjectsInDB(QSqlQuery query);
        void insertReferences(QSqlQuery query);

        void addObject(RBKit::ObjectDetailPtr objectPtr);
        void updateObject(RBKit::ObjectDetailPtr object);
        void removeObject(quint64 key);
        void reset();
        bool hasKey(quint64 key) const;
        QList<quint64> keys() const;

        void updateObjectGeneration();

        inline QHash<QString, quint64> youngGenStats() const {
            return generationStats(0, 2);
        }

        inline QHash<QString, quint64> secondGenStats() const {
            return generationStats(2, 4);
        }

        inline QHash<QString, quint64> oldGenStats() const {
            return generationStats(4, 100000);
        }

        quint32 getObjectTypeCount(const QString& className);
        quint32 liveObjectCount() const;
        const QVariantMap getObjectTypeCountMap();
        std::list<QString> sort(int critirea) const;

    private:
        // follows half-open series convention: [begin, end)
        QHash<QString, quint64> generationStats(int begin, int end) const;
    };
}

Q_DECLARE_METATYPE(RBKit::ObjectStore)


#endif // OBJECTSTORE_H
