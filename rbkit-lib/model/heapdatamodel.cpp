#include "heapdatamodel.h"
#include "heap_item_types/baseheapitem.h"

namespace RBKit {
HeapDataModel::HeapDataModel(BaseHeapItem *_item, QObject *parent)
    : QAbstractItemModel(parent), rootItem(_item)
{
}

HeapDataModel::~HeapDataModel() {
    // delete items;
}

QVariant HeapDataModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    BaseHeapItem *item = static_cast<BaseHeapItem *>(index.internalPointer());
    switch(role) {
    case Qt::DisplayRole:
        return item->data(index.column());
        break;
    case Qt::ToolTipRole:
        return QVariant(item->leadingIdentifier());
        break;
    default:
        return QVariant();
    }
}

Qt::ItemFlags HeapDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant HeapDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return QVariant(QString("Class"));
        case 1:
            return QVariant(QString("Count"));
        case 2:
            return QVariant(QString("Reference Count"));
        case 3:
            return QVariant(QString("Count Percentage"));
        case 4:
            return QVariant(QString("Object Size (bytes)"));
        }
    }
    return QVariant();
}

QModelIndex HeapDataModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    BaseHeapItem *parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<BaseHeapItem *>(parent.internalPointer());
    }

    BaseHeapItem *childItem = parentItem->getChild(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex HeapDataModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    BaseHeapItem *childItem = static_cast<BaseHeapItem *>(child.internalPointer());
    BaseHeapItem *parentItem = childItem->getParent();

    if (parentItem == rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

bool HeapDataModel::hasChildren(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return true;
    else {
        BaseHeapItem *item = static_cast<BaseHeapItem *>(parent.internalPointer());
        return item->hasChildren();
    }
}

bool HeapDataModel::canFetchMore(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return true;
    else {
        BaseHeapItem *item = static_cast<BaseHeapItem *>(parent.internalPointer());
        return item->hasChildren();
    }
}

void HeapDataModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid()) {
        int row = parent.row();
        int startRow = row + 1 ;
        BaseHeapItem *item = static_cast<BaseHeapItem *>(parent.internalPointer());
        if (!item->childrenFetched) {
            item->fetchChildren();
        }
    }
}

int HeapDataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    if(!parent.isValid())
        return rootItem->childrenCount();
    else {
        BaseHeapItem *item = static_cast<BaseHeapItem *>(parent.internalPointer());
        if (item->childrenFetched)
            return item->childrenCount();
        else
            return 0;
    }

}

int HeapDataModel::columnCount(const QModelIndex &parent) const
{
   return 5;
}


}
