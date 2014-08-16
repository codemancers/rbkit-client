#include "heaptable.h"

HeapTable::HeapTable(QObject *parent):
    QAbstractTableModel(parent)
{
}


HeapTable::HeapTable(QObject *parent, const RBKit::ObjectStore objectStore):
    QAbstractTableModel(parent),
    objectStore(objectStore)
{

}

int HeapTable::rowCount(const QModelIndex &parent) const
{
    return 5;
}

int HeapTable::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant HeapTable::data(const QModelIndex &index, int role) const
{
    return QVariant(QString("Hello"));
}
