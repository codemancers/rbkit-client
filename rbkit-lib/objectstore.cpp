#include "objectstore.h"

void RBKit::ObjectStore::insertObjectsInDB(QSqlQuery query)
{
    qDebug() << "*****  Insert number of  objects : " << objectStore.size();
    int counter = 0;

    for (auto& objectDetail : objectStore) {
        query.addBindValue(objectDetail->objectId);
        query.addBindValue(objectDetail->className);
        query.addBindValue(objectDetail->size);
        query.addBindValue(objectDetail->references.size());
        query.addBindValue(objectDetail->getFileLine());
        qDebug() << "Inserting object with id : " << objectDetail->objectId << " count : " << counter;
        if (!query.exec()) {
            qDebug() << query.lastError();
        }

        ++counter;
    }
}

void RBKit::ObjectStore::insertReferences(QSqlQuery query)
{
    for (auto& objectDetail : objectStore) {
        QList<quint64> references = objectDetail->references;
        if (!references.isEmpty()) {
            for(auto refId : references) {
                query.addBindValue(objectDetail->objectId);
                query.addBindValue(refId);
                if (!query.exec()) {
                    qDebug() << "Inserting relations failed";
                    qDebug() << query.lastError();
                }
            }
        }
    }
}


void RBKit::ObjectStore::addObject(RBKit::ObjectDetailPtr objectDetail)
{
    objectStore[objectDetail->objectId] = objectDetail;
    ++objectTypeCount[objectDetail->className];
}

void RBKit::ObjectStore::removeObject(quint64 key)
{
    auto iter = objectStore.find(key);
    if (iter != objectStore.end()) {
        auto object = iter.value();

        quint32 oldCount = objectTypeCount[object->className];
        if(oldCount > 0) {
            oldCount -= 1;
        }
        objectTypeCount[object->className] = oldCount;
    }
    objectStore.remove(key);
}

void RBKit::ObjectStore::reset() {
    objectStore.clear();
    objectTypeCount.clear();
}

bool RBKit::ObjectStore::hasKey(quint64 key) const
{
    return objectStore.find(key) != objectStore.end();
}

QList<quint64> RBKit::ObjectStore::keys() const
{
    return objectStore.keys();
}

void RBKit::ObjectStore::updateObject(RBKit::ObjectDetailPtr newObject)
{
    // we should always store object generation info properly when updating.
    RBKit::ObjectDetailPtr oldObject = objectStore[newObject->objectId];
    newObject->objectGeneration = oldObject->objectGeneration;
    objectStore[newObject->objectId] = newObject;
}


void RBKit::ObjectStore::updateObjectGeneration()
{
    for (auto& object : objectStore) {
        object->updateGeneration();
    }
}

QHash<QString, quint64> RBKit::ObjectStore::generationStats(int begin, int end) const
{
    QHash<QString, quint64> stats;
    for (const auto& iter : objectStore) {
        if (begin <= iter->objectGeneration && iter->objectGeneration < end) {
            ++stats[iter->className];
        }
    }

    return stats;
}


quint32 RBKit::ObjectStore::getObjectTypeCount(const QString &className)
{
    return objectTypeCount[className];
}

quint32 RBKit::ObjectStore::liveObjectCount() const
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

std::list<QString> RBKit::ObjectStore::sort(int critirea) const
{
    QMap<QString, quint32> map;
    std::list<QString> classNames = objectTypeCount.keys().toStdList();
    Sorter s(this);
    classNames.sort(s);
    return classNames;
}
