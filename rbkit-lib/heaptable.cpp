#include "heaptable.h"

HeapTable::HeapTable(QObject *parent):
    QAbstractTableModel(parent)
{
}


HeapTable::HeapTable(QObject *parent, const RBKit::ObjectStore _objectStore):
    QAbstractTableModel(parent),
    objectStore(_objectStore)
{
    std::list<QString> sortedObjectList = objectStore.sort(0);
    for (std::list<QString>::iterator it = sortedObjectList.begin(); it != sortedObjectList.end(); ++it)
        sortedObjectKeys.push_back(*it);
}

int HeapTable::rowCount(const QModelIndex &parent) const
{
    return objectStore.objectTypeCount.size();
}

int HeapTable::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant HeapTable::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole){
        QString objectType = sortedObjectKeys[index.row()];
        switch (index.column()) {
        case 0:
            return objectType;
        case 1:
            return objectStore.objectTypeCount[objectType];
        }
    }
    return QVariant();
}

QVariant HeapTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Horizontal) {
                switch (section)
                {
                case 0:
                    return QString("Class");
                case 1:
                    return QString("Count");
                }
            }
        }
        return QVariant();
}
