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
    int objectTypeCount = objectStore.objectTypeCount.size();
    if(objectTypeCount < 30)
        return objectTypeCount;
    else
        return 30;
}

int HeapTable::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant HeapTable::data(const QModelIndex &index, int role) const
{
    return QVariant(QString("Hello"));
}

QVariant HeapTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(section) {
    case 0:
        return QString("Class");
    case 1:
        return QString("Count");
    case 2:
        return QString("Percentage");
    case 3:
        return QString("Retention");
    case 4:
        return QString("Retention Percentage");
    default:
        return QString("Ho HO");
    }
}
