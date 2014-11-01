#ifndef RBKIT_HEAPITEMDETAIL_H
#define RBKIT_HEAPITEMDETAIL_H

#include <QString>
#include <QVector>

namespace RBKit {

class BaseHeapItem;

class HeapItemDetail
{
public:
    HeapItemDetail(const QString _className, const QString _fileName);
    const QString className;
    const QString fileName;
    QVector<BaseHeapItem *> parents;

    void addParent(BaseHeapItem *parent);
    void populateDetails(BaseHeapItem *rootObject, BaseHeapItem *child);
};

} // namespace RBKit

#endif // RBKIT_HEAPITEMDETAIL_H
