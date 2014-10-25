#ifndef RBKIT_HEAPITEM_H
#define RBKIT_HEAPITEM_H

#include <QUuid>
#include <QCryptographicHash>
#include "../objectstore.h"
#include "baseheapitem.h"

namespace RBKit {

class DiffItem;

class HeapItem : public BaseHeapItem
{
public:
    HeapItem(int _snapShotVersion);
    HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion);
    virtual ~HeapItem();

    virtual bool hasChildren();
    virtual quint32 childrenCount();
    virtual void fetchChildren();

    virtual BaseHeapItem *getChild(int index);
    virtual void addChildren(BaseHeapItem *item);

    virtual void computePercentage();
    virtual int row();
    HeapItem *getSelectedReferences();
    DiffItem *minus(HeapItem *other);
    virtual void getObjectParents();
};

} // namespace RBKit

#endif // RBKIT_HEAPITEM_H
