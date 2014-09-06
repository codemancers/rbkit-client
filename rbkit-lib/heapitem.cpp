#include "heapitem.h"

namespace RBKit {

HeapItem::HeapItem(int _snapShotVersion)
    : snapShotVersion(_snapShotVersion)
{
    parent = 0;
    leafNode = false;
    childrenFetched = false;
    childrenCountFetched = -1;
}

HeapItem::HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion)
    : className(_className), count(_count),
      referenceCount(_referenceCount), totalSize(_totalSize)
    , snapShotVersion(_snapShotVersion)
{
    parent = 0;
    leafNode = false;
    childrenFetched = false;
    childrenCountFetched = -1;
}

HeapItem::HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, const QString _filename, int _snapShotVersion)
    : className(_className), count(_count),
      referenceCount(_referenceCount), totalSize(_totalSize), filename(_filename)
    , snapShotVersion(_snapShotVersion)
{
    parent = 0;
    leafNode = true;
    childrenFetched = false;
    childrenCountFetched = -1;
}

HeapItem::~HeapItem()
{
    if (!children.isEmpty()) {
        for(auto child : children) {
            delete child;
        }
    }
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

QString HeapItem::leadingIdentifier()
{
    if (leafNode) {
        if (filename.isEmpty()) {
            return className;
        } else {
            return QString("%0 - %1").arg(className).arg(filename);
        }
    } else {
        return className;
    }
}

HeapItem *HeapItem::getSelectedReferences()
{
    QString queryString;
    if (leafNode) {
        queryString = QString("select rbkit_objects_%0.class_name, count(rbkit_objects_%0.id) as total_count, sum(rbkit_objects_%0.reference_count) as ref_count, "
                            "sum(rbkit_objects_%0.size) as total_size from rbkit_objects_%0 where rbkit_objects_%0.id in "
                            "(select rbkit_object_references_%0.child_id from rbkit_object_references_%0 "
                            " INNER JOIN rbkit_objects_%0 ON rbkit_objects_%0.id = rbkit_object_references_%0.object_id "
                            " where rbkit_objects_%0.class_name = '%1' and rbkit_objects_%0.file='%2')"
                            " group by rbkit_objects_%0.class_name").arg(snapShotVersion).arg(className).arg(filename);
    } else {
        queryString = QString("select rbkit_objects_%0.class_name, count(rbkit_objects_%0.id) as total_count, sum(rbkit_objects_%0.reference_count) as ref_count, "
                            "sum(rbkit_objects_%0.size) as total_size from rbkit_objects_%0 where rbkit_objects_%0.id in "
                            "(select rbkit_object_references_%0.child_id from rbkit_object_references_%0 "
                            " INNER JOIN rbkit_objects_%0 ON rbkit_objects_%0.id = rbkit_object_references_%0.object_id "
                            " where rbkit_objects_%0.class_name = '%1')"
                            " group by rbkit_objects_%0.class_name").arg(snapShotVersion).arg(className);
    }

    QSqlQuery query(queryString);
    HeapItem *rootItem = new HeapItem(snapShotVersion);

    while(query.next()) {
        HeapItem* item = new HeapItem(query.value(0).toString(), query.value(1).toInt(),
                                      query.value(2).toInt(), query.value(3).toInt(), snapShotVersion);
        rootItem->addChildren(item);
    }
    rootItem->childrenFetched = true;
    rootItem->computePercentage();
    return rootItem;
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


bool HeapItem::hasChildren()
{
    if (leafNode)
        return false;
    if (childrenCountFetched > -1) {
        if (childrenCountFetched > 1)
            return true;
        else
            return false;
    }

    QSqlQuery query(QString("select count(*) as count from rbkit_objects_%0 where class_name='%1'").arg(snapShotVersion).arg(className));
    qDebug() << "Checking for children for class : " << className;
    while(query.next()) {
        childrenCountFetched = query.value(0).toInt();
    }

    if (childrenCountFetched > 1)
        return true;
    else
        return false;
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
