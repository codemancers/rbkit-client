#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include "objectdetail.h"
#include <QMap>

class ObjectStore
{
public:
    ObjectStore();
    QMap<quint64, ObjectDetail> objectStore;
    void addObject(const ObjectDetail& object);
    const ObjectDetail getObject(quint64);
};


#endif // OBJECTSTORE_H
