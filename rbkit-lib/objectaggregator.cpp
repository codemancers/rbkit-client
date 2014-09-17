#include "objectaggregator.h"
#include <QDebug>


RBKit::ObjectAggregator::ObjectAggregator()
    : totalObjects(0)
    , totalHeapSize(0)
    , totalMemSize(0)
{
}


void RBKit::ObjectAggregator::objCreated(RBKit::ObjectDetailPtr object)
{
    idToName[object->objectId] = object->className;
    ++typeToCount[object->className];
    ++totalObjects;
}

void RBKit::ObjectAggregator::objDeleted(quint64 key)
{
    auto className = idToName[key];
    if (idToName.remove(key)) {
        if (typeToCount[className] > 0) {
            --typeToCount[className];
        }

        if (totalObjects > 0) {
            --totalObjects;
        }
    }
}

void RBKit::ObjectAggregator::
updateFromSnapshot(const QList<RBKit::ObjectDetailPtr>& objects)
{
    typeToCount.clear();
    idToName.clear();

    for (auto& object : objects) {
        idToName[object->objectId] = object->className;
        ++typeToCount[object->className];
    }

    totalObjects = objects.size();
}

void RBKit::ObjectAggregator::onGcStats(const QVariantMap& map)
{
    totalHeapSize = map["total_heap_size"].toULongLong();
    totalMemSize = map["total_memsize"].toULongLong();
}

QHash<QString, double> RBKit::ObjectAggregator::liveStats() const
{
    QHash<QString, double> hash;

    hash["Heap Objects"] = double(totalObjects)/1000;
    hash["Heap Size"] = double(totalHeapSize)/(1024*1024);
    hash["Mem Size"] = double(totalMemSize)/(1024*1024);

    return hash;
}
