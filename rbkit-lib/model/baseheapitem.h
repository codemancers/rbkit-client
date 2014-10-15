#ifndef RBKIT_ABSTRACTHEAPITEM_H
#define RBKIT_ABSTRACTHEAPITEM_H

#include <QVector>

namespace RBKit {
class BaseHeapItem
{
public:
    BaseHeapItem();
    QString className;
    quint32 count;
    quint32 referenceCount;
    quint32 totalSize;
    QString filename;
    BaseHeapItem* parent;
    bool childrenFetched;
    int countPercentage;
    int childrenCountFetched;
    int refPercentage;
    int snapShotVersion;
    QVector<BaseHeapItem *> children;
    QString objectsTableName;
    QString referenceTableName;

    virtual bool hasChildren() = 0;
    virtual quint32 childrenCount() = 0;
    virtual void fetchChildren() = 0;
    virtual int row() = 0;

    virtual QVariant getClassOrFile() const;

    // return child at given position
    BaseHeapItem *getChild(int index) = 0;

    virtual const QString toString() const;

    QVariant data(int column) const;
    virtual void addChildren(BaseHeapItem *item) = 0;

    virtual QString leadingIdentifier();

    virtual void computePercentage() = 0;
    virtual BaseHeapItem *getSelectedReferences() = 0;
    virtual QString shortLeadingIdentifier();
    virtual BaseHeapItem *minus(BaseHeapItem *other) = 0;

    // getters and setters for table names
    QString getObjectsTableName() const;
    void setObjectsTableName(const QString &value);
    QString getReferenceTableName() const;
    void setReferenceTableName(const QString &value);

    // parent getters and setters
    BaseHeapItem *getParent() const;
    void setParent(BaseHeapItem *value);
};

} // namespace RBKit

#endif // RBKIT_ABSTRACTHEAPITEM_H
