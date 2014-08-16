#include "objectstore.h"

RBKit::ObjectStore::ObjectStore()
{
}

void RBKit::ObjectStore::addObject(RBKit::ObjectDetail *objectDetail)
{
    objectStore[objectDetail->objectId] = objectDetail;
    ++objectTypeCount[objectDetail->className];
}

void RBKit::ObjectStore::removeObject(quint64 key)
{
    RBKit::ObjectDetail *objectDetail = getObject(key);
    if(objectDetail != NULL) {
        quint32 oldCount = objectTypeCount[objectDetail->className];
        if(oldCount > 0) {
            oldCount -= 1;
        }
        objectTypeCount[objectDetail->className] = oldCount;
    }
    objectStore.remove(key);
}

void RBKit::ObjectStore::reset() {
    objectStore.clear();
    objectTypeCount.clear();
}

void RBKit::ObjectStore::updateObjectGeneration()
{
    QHash<quint64, ObjectDetail*>::const_iterator iter = objectStore.constBegin();
    while(iter != objectStore.constEnd()) {
        ObjectDetail detail = iter.value();
        detail.updateGeneration();
    }
}

RBKit::ObjectDetail *RBKit::ObjectStore::getObject(quint64 key)
{
    QHash<quint64, RBKit::ObjectDetail*>::const_iterator i = objectStore.find(key);
    if(i != objectStore.end()) {
        return i.value();
    } else {
        return NULL;
    }
}

quint32 RBKit::ObjectStore::getObjectTypeCount(const QString &className)
{
    return objectTypeCount[className];
}

quint32 RBKit::ObjectStore::liveObjectCount()
{
    return objectStore.size();
}

const QVariantMap RBKit::ObjectStore::getObjectTypeCountMap()
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
