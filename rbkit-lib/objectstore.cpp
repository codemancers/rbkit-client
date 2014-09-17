#include "objectstore.h"
#include "objectaggregator.h"

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
    aggregator.objCreated(objectDetail);
}

void RBKit::ObjectStore::removeObject(quint64 key)
{
    aggregator.objDeleted(key);
    objectStore.remove(key);
}

void RBKit::ObjectStore::reset() {
    objectStore.clear();
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


void RBKit::ObjectStore::updateFromSnapshot(const QList<RBKit::ObjectDetailPtr>& objects)
{
    QHash<quint64, RBKit::ObjectDetailPtr> newStore;

    for (auto& object : objects) {
        auto objectId = object->objectId;
        newStore[objectId] = object;

        auto oldObjectIter = objectStore.find(objectId);
        if (oldObjectIter != objectStore.end()) {
            // retain the object generation from old object
            auto oldGeneration = oldObjectIter.value()->objectGeneration;
            object->objectGeneration = oldGeneration;
        }
    }

    objectStore.swap(newStore);

    // update aggregator also.
    aggregator.updateFromSnapshot(objects);
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
