#ifndef RBKIT_HEAPITEM_H
#define RBKIT_HEAPITEM_H

#include <QUuid>
#include <QCryptographicHash>
#include "objectstore.h"

namespace RBKit {

class DiffItem;

class HeapItem
{
    void initializeDataMembers();
public:
    HeapItem(int _snapShotVersion);
    HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion);
    HeapItem(const QString _className, quint32 _count, quint32 _referenceCount,
             quint32 _totalSize, const QString _filename, int _snapShotVersion);
    virtual ~HeapItem();


    bool hasChildren();
    quint32 childrenCount();
    void fetchChildren();
    HeapItem *getParent() const;
    HeapItem *getChild(int index);
    void setParent(HeapItem *value);
    const QString toString() const;
    QVariant data(int column) const;
    void addChildren(HeapItem *item);
    void computePercentage();
    QVariant getClassOrFile() const;
    int row();
    QString leadingIdentifier();
    HeapItem *getSelectedReferences();
    QString getObjectsTableName() const;
    void setObjectsTableName(const QString &value);
    QString getReferenceTableName() const;
    void setReferenceTableName(const QString &value);
    QString shortLeadingIdentifier();
    DiffItem *minus(HeapItem *other);
    bool getIsSnapshot() const;
    void setIsSnapshot(bool value);
    virtual void getObjectParents();
};

} // namespace RBKit

#endif // RBKIT_HEAPITEM_H
