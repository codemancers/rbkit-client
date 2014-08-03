#include "objectstore.h"

ObjectStore::ObjectStore()
{
}

void ObjectStore::addObject(const ObjectDetail &object)
{
   this->objectStore[object.objectId] = object;
}

const ObjectDetail ObjectStore::getObject(quint64 key)
{
   return objectStore.value(key, NULL);
}
