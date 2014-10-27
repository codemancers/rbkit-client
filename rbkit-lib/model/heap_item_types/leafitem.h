#ifndef RBKIT_LEAFITEM_H
#define RBKIT_LEAFITEM_H

#include "baseheapitem.h"

namespace RBKit {


class LeafItem : public RBKit::BaseHeapItem
{
public:
    LeafItem(const QString _className, quint32 _count, quint32 _referenceCount,
             quint32 _totalSize, const QString _filename, int _snapShotVersion);

    virtual QString leadingIdentifier();
    virtual QVariant getClassOrFile() const;
    virtual BaseHeapItem *getSelectedReferences();
};

} // namespace RBKit

#endif // RBKIT_LEAFITEM_H
