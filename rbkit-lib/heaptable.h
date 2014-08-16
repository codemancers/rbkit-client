#ifndef HEAPTABLE_H
#define HEAPTABLE_H

#include <QAbstractTableModel>
#include "objectstore.h"
#include "objectdetail.h"

class HeapTable : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit HeapTable(QObject *parent);
    HeapTable(QObject *parent, const RBKit::ObjectStore objectStore);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private:
    const RBKit::ObjectStore objectStore;
};

#endif // HEAPTABLE_H
