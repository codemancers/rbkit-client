#ifndef RBKIT_DIFFITEM_H
#define RBKIT_DIFFITEM_H

#include "heapitem.h"

namespace RBKit {

class DiffItem : public HeapItem
{
    using HeapItem::HeapItem;
public:
    void getObjectParents();
};

} // namespace RBKit

#endif // RBKIT_DIFFITEM_H
