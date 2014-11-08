#include "baseheapitem.h"
#include <QSqlQuery>
#include "heapitem.h"
#include "stringutil.h"
#include <QFileInfo>

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
    objectParent = 0;
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
    objectParent = 0;
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
    objectParent = 0;
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

BaseHeapItem *BaseHeapItem::getObjectParents(BaseHeapItem *rootItem)
{
    if (objectParent != NULL) {
        return objectParent;
    } else {
        QString queryString;
        QString viewName = QString("view_").append(RBKit::StringUtil::randomSHA());
        queryString = QString("create view %0 AS select * from %1 where %1.id in "
                              "(select %2.object_id from %2 "
                              " INNER JOIN %3 ON %3.id = %2.child_id "
                              " where %3.class_name = '%4' and %3.file='%5')").
                arg(viewName).arg(rootItem->originalObjectsTableName).
                arg(rootItem->referenceTableName).arg(rootItem->objectsTableName).
                arg(className).arg(filename);

        QSqlQuery query;

        if (!query.exec(queryString)) {
            qDebug() << "Error creating view with";
            qDebug() << query.lastError();
        }

        objectParent = new HeapItem(-1);
        objectParent->setObjectsTableName(viewName);
        objectParent->setIsSnapshot(false);
        objectParent->findImmediateChildren();
        objectParent->childrenFetched = true;
        objectParent->computePercentage();
        return objectParent;
    }
}

QString BaseHeapItem::lazyLoadObjectFileName()
{
    QSqlQuery searchQuery(
                QString("select file from %0 where class_name = '%1'").arg(objectsTableName).arg(className));

    while(searchQuery.next()) {
        return searchQuery.value(0).toString();
    }
    return QString("<compiled>");
}

QString BaseHeapItem::fullFileName()
{
    if (filename.isEmpty()) {
        filename = lazyLoadObjectFileName();
    }
    QFileInfo fileInfo(filename.split(":").at(0));
    return fileInfo.canonicalFilePath();
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
    if (filename.isEmpty()) {
        if (count > 1)
            return className;
        else {
            filename = lazyLoadObjectFileName();
            return filename;
        }
    } else {
        return QString("%0 - %1").arg(className).arg(filename);
    }
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
        int objectCount = searchQuery.value(1).toInt();
        BaseHeapItem* item = new HeapItem(searchQuery.value(0).toString(), objectCount,
                                      searchQuery.value(2).toInt(), searchQuery.value(3).toInt(), -1);
        item->setObjectsTableName(objectsTableName);
        addChildren(item);
    }
}

QVariant BaseHeapItem::getClassOrFile() const
{
    if (filename.isEmpty()) {
        return className;
    } else {
        QString shortFileName = QFileInfo(filename).fileName();
        return QString("%0 - %1").arg(className).arg(shortFileName);
    }
}

BaseHeapItem *BaseHeapItem::getChild(int index)
{
    return NULL;
}

} // namespace RBKit
