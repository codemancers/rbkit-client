#include "heapitem.h"

namespace RBKit {

HeapItem::HeapItem(int _snapShotVersion)
    : snapShotVersion(_snapShotVersion)
{
    parent = 0;
    leafNode = false;
    childrenFetched = false;
}

HeapItem::HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion)
    : className(_className), count(_count),
      referenceCount(_referenceCount), totalSize(_totalSize)
    , snapShotVersion(_snapShotVersion)
{
    parent = 0;
    leafNode = false;
    childrenFetched = false;
}

HeapItem::HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, const QString _filename, int _snapShotVersion)
    : className(_className), count(_count),
      referenceCount(_referenceCount), totalSize(_totalSize), filename(_filename)
    , snapShotVersion(_snapShotVersion)
{
    parent = 0;
    leafNode = true;
    childrenFetched = false;
}

const QString HeapItem::toString() const
{
   QString string("class : %0, count : %1, ref Count : %2, size : %3");
   QString resultString = string.arg(className).arg(count).arg(referenceCount).arg(totalSize);
   return resultString;
}

QVariant HeapItem::data(int column) const
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
        return QVariant(refPercentage);
        break;
    default:
        return QVariant();
    }
}

void HeapItem::addChildren(HeapItem *item)
{
    item->setParent(this);
    children.push_back(item);
}

void HeapItem::computePercentage()
{
    int totalCount = 0;

    for(auto childItem : children) {
        totalCount += childItem->count;
    }

    for(auto childItem : children) {
        childItem->countPercentage = (childItem->count * 100.00)/totalCount;
        childItem->refPercentage = (childItem->referenceCount * 100.00)/totalCount;
    }
}

QVariant HeapItem::getClassOrFile() const
{
    if (!leafNode)
        return QVariant(className);
    else {
        if (filename.isEmpty())
            return QVariant(QString("<compiled>"));
        else
            return QVariant(filename);
    }
}

int HeapItem::row()
{
    return parent->children.indexOf(this);
}

HeapItem *HeapItem::getParent() const
{
    return parent;
}

HeapItem *HeapItem::getChild(int index)
{
   if (childrenFetched && index < children.size()) {
       return const_cast<HeapItem *>(children.at(index));
   } else {
       return NULL;
   }
}

void HeapItem::setParent(HeapItem *value)
{
    parent = const_cast<HeapItem *>(value);
}


bool HeapItem::hasChildren() const
{
    if (leafNode)
        return false;
    QSqlQuery query(QString("select count(*) as count from rbkit_objects_%0 where class_name='%1'").arg(snapShotVersion).arg(className));
    int recordCount;
    while(query.next()) {
        recordCount = query.value(0).toInt();
    }
    qDebug() << query.lastError();
    if (recordCount > 1) {
        return true;
    } else {
        return false;
    }
}

quint32 HeapItem::childrenCount()
{
    if (!childrenFetched)
        fetchChildren();
    return children.size();
}

void HeapItem::fetchChildren()
{
    if (childrenFetched)
        return;
     QSqlQuery searchQuery(QString("select file, count(id) as object_count, "
                           "sum(reference_count) as total_ref_count, sum(size) as total_size from rbkit_objects_%0 where class_name='%1' group by (file)").arg(snapShotVersion).arg(className));


     qDebug() << searchQuery.lastError();
     childrenFetched = true;
     while(searchQuery.next()) {
         HeapItem* item = new HeapItem(className, searchQuery.value(1).toInt(),
                                             searchQuery.value(2).toInt(), searchQuery.value(3).toInt(), searchQuery.value(0).toString(), snapShotVersion);
         item->setParent(this);
         children.push_back(item);
     }
     computePercentage();
}

} // namespace RBKit
