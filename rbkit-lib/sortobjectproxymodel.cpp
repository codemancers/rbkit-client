#include "sortobjectproxymodel.h"

SortObjectProxyModel::SortObjectProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool SortObjectProxyModel::hasChildren(const QModelIndex &parent) const
{
    const QModelIndex sourceIndex = mapToSource(parent);
    return sourceModel()->hasChildren(sourceIndex);
}

int SortObjectProxyModel::rowCount(const QModelIndex &parent) const
{
    const QModelIndex sourceIndex = mapToSource(parent);
    return sourceModel()->rowCount(sourceIndex);
}

bool SortObjectProxyModel::canFetchMore(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return true;
    else {
        const QModelIndex sourceIndex = mapToSource(parent);
        return sourceModel()->canFetchMore(sourceIndex);
    }
}

void SortObjectProxyModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid() && parent.column() == 0) {
        int row = parent.row();
        int startRow = row + 1 ;
        const QModelIndex sourceIndex = mapToSource(parent);
        RBKit::HeapItem *item = static_cast<RBKit::HeapItem *>(sourceIndex.internalPointer());
        if (!item->childrenFetched) {
            qDebug() << "Insert New Rows at :" << startRow << " ending at : " << startRow + item->childrenCount();
            beginInsertRows(parent, startRow, startRow + item->childrenCount());
            item->fetchChildren();
            endInsertRows();
        }
    }
}
