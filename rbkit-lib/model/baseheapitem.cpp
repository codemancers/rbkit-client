#include "baseheapitem.h"

namespace RBKit {

BaseHeapItem::BaseHeapItem()
{
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

QString BaseHeapItem::leadingIdentifier()
{
    return className;
}

QString BaseHeapItem::shortLeadingIdentifier()
{
    return className;
}

QVariant BaseHeapItem::getClassOrFile() const
{
    if (filename.isEmpty())
        return QVariant(QString("<compiled>"));
    else
        return QVariant(filename);
}

} // namespace RBKit
