#ifndef HEAPTABLE_H
#define HEAPTABLE_H

#include <QAbstractTableModel>
#include <QMap>
#include <QDebug>

#include "objectstore.h"
#include "objectdetail.h"
#include "heapitem.h"

namespace RBKit {

class HeapDataModel : public QAbstractItemModel
{
    Q_OBJECT
    HeapItem* rootItem;
public:
    HeapDataModel(HeapItem * _rootItem, QObject* parent);
    ~HeapDataModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    bool hasChildren(const QModelIndex &parent) const;
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
};

}

#endif // HEAPTABLE_H
