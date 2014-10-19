#ifndef RBKIT_HEAPITEM_H
#define RBKIT_HEAPITEM_H

#include <QUuid>
#include <QCryptographicHash>
#include "objectstore.h"
#include "baseheapitem.h"

namespace RBKit {

class DiffItem;

class HeapItem : public BaseHeapItem
{
public:
    HeapItem(int _snapShotVersion);
    HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion);
    HeapItem(const QString _className, quint32 _count, quint32 _referenceCount,
             quint32 _totalSize, const QString _filename, int _snapShotVersion);
    virtual ~HeapItem();

    virtual bool hasChildren();
    virtual quint32 childrenCount();
    virtual void fetchChildren();

    virtual BaseHeapItem *getChild(int index);
    virtual void addChildren(BaseHeapItem *item);

    virtual void computePercentage();
    virtual int row();
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
