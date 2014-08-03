#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include "objectdetail.h"
#include <QHash>

class ObjectStore
{
public:
    ObjectStore();
    QHash<quint64, ObjectDetail*> objectStore;
    void addObject(ObjectDetail *objectDetails);
    ObjectDetail *getObject(quint64 key);
};

#endif // OBJECTSTORE_H
