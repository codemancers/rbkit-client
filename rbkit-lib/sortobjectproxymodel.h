#ifndef SORTOBJECTPROXYMODEL_H
#define SORTOBJECTPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class SortObjectProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortObjectProxyModel(QObject *parent = 0);
    bool hasChildren(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
};

#endif // SORTOBJECTPROXYMODEL_H
