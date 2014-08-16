#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include "objectdetail.h"
#include <QHash>
#include <QVariant>
#include <QVariantMap>

namespace RBKit {
    class ObjectStore
    {
    public:
        ObjectStore();
        ObjectStore(const ObjectStore &);
        // Store mapping between object-id and detail
        QHash<quint64, RBKit::ObjectDetail*> objectStore;
        // mapping between object class and its count
        QHash<QString, quint32> objectTypeCount;


        void addObject(RBKit::ObjectDetail *objectDetails);
        void removeObject(quint64 key);
        void reset();
        void updateObjectGeneration();
        ObjectDetail *getObject(quint64 key);
        quint32 getObjectTypeCount(const QString& className);
        const quint32 liveObjectCount() const;
        const QVariantMap getObjectTypeCountMap();
    };
}

Q_DECLARE_METATYPE(RBKit::ObjectStore)


#endif // OBJECTSTORE_H
