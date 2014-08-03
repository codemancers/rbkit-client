#include "objectstore.h"

ObjectStore::ObjectStore()
{
}

void ObjectStore::addObject(ObjectDetail *objectDetail)
{
  objectStore[objectDetail->objectId] = objectDetail;
}

ObjectDetail *ObjectStore::getObject(quint64 key)
{
    QHash<quint64, ObjectDetail*>::const_iterator i = objectStore.find(key);
    if(i != objectStore.end()) {
        return i.value();
    } else {
        return NULL;
    }
}
