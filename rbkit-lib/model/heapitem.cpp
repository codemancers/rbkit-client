#include "heapitem.h"
#include "stringutil.h"
#include "diffitem.h"
#include <QFileInfo>

namespace RBKit {

HeapItem::HeapItem(int _snapShotVersion)
    : snapShotVersion(_snapShotVersion),
      BaseHeapItem()
{
}

HeapItem::HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion)
    : className(_className), count(_count),
      referenceCount(_referenceCount), totalSize(_totalSize), snapShotVersion(_snapShotVersion),
      BaseHeapItem()
{
}

HeapItem::HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, const QString _filename, int _snapShotVersion)
    : className(_className), count(_count), referenceCount(_referenceCount),
      totalSize(_totalSize), filename(_filename), snapShotVersion(_snapShotVersion),
      BaseHeapItem()
{
}

HeapItem::~HeapItem()
{
    if (!children.isEmpty()) {
        for(auto child : children) {
            delete child;
        }
    }
}
bool HeapItem::getIsSnapshot() const
{
    return isSnapshot;
}

void HeapItem::setIsSnapshot(bool value)
{
    isSnapshot = value;
}

QString HeapItem::getReferenceTableName() const
{
    return referenceTableName;
}

void HeapItem::setReferenceTableName(const QString &value)
{
    referenceTableName = value;
}

QString HeapItem::shortLeadingIdentifier()
{
    if (leafNode) {
        if (filename.isEmpty()) {
            return className;
        } else {
            QString shortFileName = QFileInfo(filename).fileName();
            return QString("%0 - %1").arg(className).arg(shortFileName);
        }
    } else {
        return className;
    }
}

DiffItem *HeapItem::minus(HeapItem *other)
{
    if (other->getIsSnapshot() && isSnapshot) {
        QString queryString;
        QString viewName = QString("view_").append(RBKit::StringUtil::randomSHA());
        queryString = QString("create view %0 AS select * from %1 where %1.id not in "
                              "(select %2.id from %2)").arg(viewName).arg(objectsTableName).arg(other->getObjectsTableName());

        QSqlQuery query;

        if (!query.exec(queryString)) {
            qDebug() << "Error creating view with";
            qDebug() << query.lastError();
        }

        DiffItem *rootItem = new DiffItem(-1);
        rootItem->setObjectsTableName(viewName);
        rootItem->setReferenceTableName(referenceTableName);
        rootItem->setIsSnapshot(false);
        QSqlQuery searchQuery(QString("select class_name, count(id) as object_count, "
                                      "sum(reference_count) as total_ref_count, sum(size) as total_size from %0 group by (class_name)").arg(viewName));

        while(searchQuery.next()) {
            HeapItem* item = new HeapItem(searchQuery.value(0).toString(), searchQuery.value(1).toInt(),
                                          searchQuery.value(2).toInt(), searchQuery.value(3).toInt(), -1);
            item->setObjectsTableName(viewName);
            rootItem->addChildren(item);
        }
        rootItem->childrenFetched = true;
        rootItem->computePercentage();
        return rootItem;
    } else {
        return NULL;
    }

}

QString HeapItem::getObjectsTableName() const
{
    return objectsTableName;
}

void HeapItem::setObjectsTableName(const QString &value)
{
    objectsTableName = value;
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
        return QVariant(totalSize);
        break;
    default:
        return QVariant();
    }
}

void HeapItem::addChildren(BaseHeapItem *item)
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

HeapItem *HeapItem::getSelectedReferences()
{
    QString queryString;
    QString viewName = QString("view_").append(RBKit::StringUtil::randomSHA());
    qDebug() << "** Generated uuid is : " << viewName;
    if (leafNode) {
        queryString = QString("create view %0 AS select * from %1 where %1.id in "
                            "(select %2.child_id from %2 "
                            " INNER JOIN %1 ON %1.id = %2.object_id "
                            " where %1.class_name = '%3' and %1.file='%4')").arg(viewName).arg(objectsTableName).arg(referenceTableName).arg(className).arg(filename);
    } else {
        queryString = QString("create view %0 AS select * from %1 where %1.id in "
                            "(select %2.child_id from %2 "
                            " INNER JOIN %1 ON %1.id = %2.object_id "
                            " where %1.class_name = '%3')").arg(viewName).arg(objectsTableName).arg(referenceTableName).arg(className);
    }

    QSqlQuery query;

    if (!query.exec(queryString)) {
        qDebug() << "Error creating view with";
        qDebug() << query.lastError();
    }


    HeapItem *rootItem = new HeapItem(-1);
    rootItem->setObjectsTableName(viewName);
    rootItem->setIsSnapshot(false);
    rootItem->findImmediateChildren();
    rootItem->childrenFetched = true;
    rootItem->computePercentage();
    return rootItem;
}

HeapItem *HeapItem::getParent() const
{
    return parent;
}

BaseHeapItem *HeapItem::getChild(int index)
{
   if (childrenFetched && index < children.size()) {
       return const_cast<BaseHeapItem *>(children.at(index));
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
    if (count > 1)
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
                           "sum(reference_count) as total_ref_count, sum(size) as total_size from %0 where class_name='%1' group by (file)").arg(objectsTableName).arg(className));


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
