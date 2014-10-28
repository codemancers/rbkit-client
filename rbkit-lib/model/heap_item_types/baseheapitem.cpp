#include "baseheapitem.h"
#include <QSqlQuery>
#include "heapitemdetail.h"

namespace RBKit {

BaseHeapItem::BaseHeapItem()
{
    parent = 0;
    childrenFetched = false;
    childrenCountFetched = -1;
    isSnapshot = false;
    objectsTableName = QString("rbkit_objects_%0").arg(snapShotVersion);
    referenceTableName = QString("rbkit_object_references_%0").arg(snapShotVersion);
    originalObjectsTableName = objectsTableName;
}

BaseHeapItem::BaseHeapItem(int _snapShotVersion)
    : snapShotVersion(_snapShotVersion)
{
    parent = 0;
    childrenFetched = false;
    childrenCountFetched = -1;
    isSnapshot = false;
    objectsTableName = QString("rbkit_objects_%0").arg(snapShotVersion);
    referenceTableName = QString("rbkit_object_references_%0").arg(snapShotVersion);
    originalObjectsTableName = objectsTableName;
}

BaseHeapItem::BaseHeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion)
    : className(_className), count(_count),
      referenceCount(_referenceCount), totalSize(_totalSize), snapShotVersion(_snapShotVersion)
{
    parent = 0;
    childrenFetched = false;
    childrenCountFetched = -1;
    isSnapshot = false;
    objectsTableName = QString("rbkit_objects_%0").arg(snapShotVersion);
    referenceTableName = QString("rbkit_object_references_%0").arg(snapShotVersion);
    originalObjectsTableName = objectsTableName;
}

BaseHeapItem::~BaseHeapItem()
{
   // NOOP
}
QString BaseHeapItem::getOriginalObjectsTableName() const
{
    return originalObjectsTableName;
}

void BaseHeapItem::setOriginalObjectsTableName(const QString &value)
{
    originalObjectsTableName = value;
}

HeapItemDetail *BaseHeapItem::getObjectParents()
{
}


bool BaseHeapItem::hasChildren()
{
    return false;
}

quint32 BaseHeapItem::childrenCount()
{
    return 0;
}

void BaseHeapItem::fetchChildren()
{
    // NOP
}

// return row number of child witin parent
int BaseHeapItem::row()
{
    return 0;
}
bool BaseHeapItem::getIsSnapshot() const
{
    return isSnapshot;
}

void BaseHeapItem::setIsSnapshot(bool value)
{
    isSnapshot = value;
}


const QString BaseHeapItem::toString() const
{
    QString string("class : %0, count : %1, ref Count : %2, size : %3");
    QString resultString = string.arg(className).arg(count).arg(referenceCount).arg(totalSize);
    return resultString;
}
BaseHeapItem *BaseHeapItem::getParent() const
{
    return parent;
}

void BaseHeapItem::setParent(BaseHeapItem *value)
{
    parent = value;
}

QString BaseHeapItem::getReferenceTableName() const
{
    return referenceTableName;
}

void BaseHeapItem::setReferenceTableName(const QString &value)
{
    referenceTableName = value;
}

QString BaseHeapItem::getObjectsTableName() const
{
    return objectsTableName;
}

void BaseHeapItem::setObjectsTableName(const QString &value)
{
    objectsTableName = value;
}


QVariant BaseHeapItem::data(int column) const
{
    switch (column) {
    case 0:
        return getClassOrFile();
        break;
    case 1:
        return QVariant(count);
        break;
    case 2:
        return QVariant(referenceCount);
        break;
    case 3:
        return QVariant(countPercentage);
        break;
    case 4:
        return QVariant(totalSize);
        break;
    default:
        return QVariant();
    }
}

void BaseHeapItem::addChildren(BaseHeapItem *item)
{
    // NOOP;
}

QString BaseHeapItem::leadingIdentifier()
{
    return className;
}

void BaseHeapItem::computePercentage()
{
    // NOOP;
}

BaseHeapItem *BaseHeapItem::getSelectedReferences()
{
    return NULL;
}

QString BaseHeapItem::shortLeadingIdentifier()
{
    return className;
}

BaseHeapItem *BaseHeapItem::minus(BaseHeapItem *other)
{
    return NULL;
}

void BaseHeapItem::findImmediateChildren()
{
    QSqlQuery searchQuery(
                QString("select class_name, count(id) as object_count, "
                        "sum(reference_count) as total_ref_count, sum(size) as total_size from %0 group by (class_name)").arg(objectsTableName));

    while(searchQuery.next()) {
        BaseHeapItem* item = new BaseHeapItem(searchQuery.value(0).toString(), searchQuery.value(1).toInt(),
                                      searchQuery.value(2).toInt(), searchQuery.value(3).toInt(), -1);
        item->setObjectsTableName(objectsTableName);
        addChildren(item);
    }
}

QVariant BaseHeapItem::getClassOrFile() const
{
    return QVariant(className);
}

BaseHeapItem *BaseHeapItem::getChild(int index)
{
    return NULL;
}

} // namespace RBKit
