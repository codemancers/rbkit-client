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
#include "objectaggregator.h"

namespace RBKit {
    class ObjectStore
    {
    public:
        // database related functions
        void insertObjectsInDB(QSqlQuery query);
        void insertReferences(QSqlQuery query);

        void addObject(RBKit::ObjectDetailPtr objectPtr);
        void updateObject(RBKit::ObjectDetailPtr object);
        void removeObject(quint64 key);
        void reset();
        bool hasKey(quint64 key) const;
        QList<quint64> keys() const;

        void updateFromSnapshot(const QList<RBKit::ObjectDetailPtr>& objects);
        bool loadPartialSnapshot(const QList<RBKit::ObjectDetailPtr>& objects, quint64 completeMessageCount);
        void updateObjectGeneration();

        inline void onGcStats(const QVariantMap& stats) {
            aggregator.onGcStats(stats);
        }

        inline QHash<QString, double> liveStats() const {
            return aggregator.liveStats();
        }

        inline QHash<QString, quint64> youngGenStats() const {
            return generationStats(0, 2);
        }

        inline QHash<QString, quint64> secondGenStats() const {
            return generationStats(2, 4);
        }

        inline QHash<QString, quint64> oldGenStats() const {
            return generationStats(4, 100000);
        }
        ObjectStore();

    private:
        // follows half-open series convention: [begin, end)
        QHash<QString, quint64> generationStats(int begin, int end) const;
        // Store mapping between object-id and detail
        QHash<quint64, RBKit::ObjectDetailPtr> objectStore;
        ObjectAggregator aggregator;
        quint64 loadedMessages;
    };
}

Q_DECLARE_METATYPE(RBKit::ObjectStore)


#endif // OBJECTSTORE_H
