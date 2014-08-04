#include "objectstore.h"

ObjectStore::ObjectStore()
{
    this->objectStore = new QHash<quint64, ObjectDetail *>();
}

void ObjectStore::addObject(ObjectDetail *objectDetail)
{
    (*objectStore)[objectDetail->objectId] = objectDetail;
    quint32 oldCount = objectTypeCount[objectDetail->className];
    oldCount += 1;
    objectTypeCount[objectDetail->className] = oldCount;
}

void ObjectStore::removeObject(quint64 key)
{
    ObjectDetail *objectDetail = getObject(key);
    if(objectDetail != NULL) {
        quint32 oldCount = objectTypeCount[objectDetail->className];
        if(oldCount > 0) {
            oldCount -= 1;
        }
        objectTypeCount[objectDetail->className] = oldCount;
    }
    objectStore->remove(key);
}

void ObjectStore::reset() {
    objectStore->clear();
    objectTypeCount.clear();
}

ObjectDetail *ObjectStore::getObject(quint64 key)
{
    QHash<quint64, ObjectDetail*>::const_iterator i = objectStore->find(key);
    if(i != objectStore->end()) {
        return i.value();
    } else {
        return NULL;
    }
}

quint32 ObjectStore::getObjectTypeCount(const QString &className)
{
    return objectTypeCount[className];
}

quint32 ObjectStore::liveObjectCount()
{
    return objectStore->size();
}

const QVariantMap ObjectStore::getObjectTypeCountMap()
{
    QVariantMap map;
    QHash<QString, quint32>::const_iterator typeIter;
    for(typeIter = objectTypeCount.begin()
        ; typeIter != objectTypeCount.end()
        ; typeIter++) {
        map[typeIter.key()] = typeIter.value();
    }
    return map;
}
