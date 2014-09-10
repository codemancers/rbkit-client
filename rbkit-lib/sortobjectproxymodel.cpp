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
