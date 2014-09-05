#ifndef RBKIT_HEAPITEM_H
#define RBKIT_HEAPITEM_H

#include "objectstore.h"

namespace RBKit {
class HeapItem
{
public:
    HeapItem(int _snapShotVersion);
    HeapItem(const QString _className, quint32 _count, quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion);
    HeapItem(const QString _className, quint32 _count, quint32 _referenceCount,
             quint32 _totalSize, const QString _filename, int _snapShotVersion);
    ~HeapItem();

    QString className;
    quint32 count;
    quint32 referenceCount;
    quint32 totalSize;
    QString filename;
    HeapItem* parent;
    bool childrenFetched;
    int countPercentage;
    int childrenCountFetched;
    int refPercentage;
    int snapShotVersion;
    bool leafNode;
    QVector<HeapItem *> children;

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
};

} // namespace RBKit

#endif // RBKIT_HEAPITEM_H
