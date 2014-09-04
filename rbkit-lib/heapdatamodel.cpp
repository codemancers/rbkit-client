#include "heapdatamodel.h"

namespace RBKit {
HeapDataModel::HeapDataModel(HeapItem * _item, QObject *parent)
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

    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    HeapItem *item = static_cast<HeapItem *>(index.internalPointer());

    qDebug() << "******** Calling data for row : " << index.row() << " and class : " << item->className;
    return item->data(index.column());
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
            return QVariant(QString("Ref Count"));
        case 3:
            return QVariant(QString("Count Percentage"));
        case 4:
            return QVariant(QString("Ref Percentage"));
        }
    }
    return QVariant();
}

QModelIndex HeapDataModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    HeapItem *parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<HeapItem *>(parent.internalPointer());
    }

    qDebug() << "Building index for : " << row << " With parent : " << parentItem->className;

    HeapItem *childItem = parentItem->getChild(row);
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

    HeapItem *childItem = static_cast<HeapItem *>(child.internalPointer());
    HeapItem *parentItem = childItem->getParent();

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
        HeapItem *item = static_cast<HeapItem *>(parent.internalPointer());
        return item->hasChildren();
    }
}

bool HeapDataModel::canFetchMore(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return true;
    else {
        HeapItem *item = static_cast<HeapItem *>(parent.internalPointer());
        return item->hasChildren();
    }
}

void HeapDataModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid()) {
        int row = parent.row();
        int startRow = row + 1 ;
        HeapItem *item = static_cast<HeapItem *>(parent.internalPointer());
        if (!item->childrenFetched) {
            qDebug() << "Insert New Rows at :" << startRow << " ending at : " << startRow + item->childrenCount();
            beginInsertRows(parent, startRow, startRow + item->childrenCount());
            item->fetchChildren();
            endInsertRows();
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
        HeapItem *item = static_cast<HeapItem *>(parent.internalPointer());
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
