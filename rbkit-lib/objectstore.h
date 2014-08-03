#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include "objectdetail.h"
#include <QHash>

class ObjectStore
{
public:
    ObjectStore();
    // Store mapping between object-id and detail
    QHash<quint64, ObjectDetail*> objectStore;
    // mapping between object class and its count
    QHash<QString, quint32> objectTypeCount;
    void addObject(ObjectDetail *objectDetails);
    void removeObject(quint64 key);
    void reset();
    ObjectDetail *getObject(quint64 key);
    quint32 getObjectTypeCount(const QString& className);
    quint32 liveObjectCount();
};

#endif // OBJECTSTORE_H
