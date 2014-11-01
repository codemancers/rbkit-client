#ifndef RBKIT_ABSTRACTHEAPITEM_H
#define RBKIT_ABSTRACTHEAPITEM_H

#include <QVector>
#include <QString>
#include <QVariant>

namespace RBKit {
class HeapItem;

class BaseHeapItem
{
public:
    BaseHeapItem();
    BaseHeapItem(int _snapShotVersion);
    BaseHeapItem(const QString _className, quint32 _count,
                 quint32 _referenceCount, quint32 _totalSize, int _snapShotVersion);
    virtual ~BaseHeapItem();
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
    bool isSnapshot;
    QVector<BaseHeapItem *> children;
    QString objectsTableName;
    QString referenceTableName;
    // contains original objects table from which this view was derived
    QString originalObjectsTableName;

    virtual bool hasChildren();
    virtual quint32 childrenCount();
    virtual void fetchChildren();
    virtual int row();

    virtual QVariant getClassOrFile() const;

    // return child at given position
    virtual BaseHeapItem *getChild(int index);

    virtual const QString toString() const;

    QVariant data(int column) const;
    virtual void addChildren(BaseHeapItem *item);

    virtual QString leadingIdentifier();

    virtual void computePercentage();
    virtual BaseHeapItem *getSelectedReferences();
    virtual QString shortLeadingIdentifier();
    virtual BaseHeapItem *minus(BaseHeapItem *other);
    virtual void findImmediateChildren();

    // getters and setters for table names
    QString getObjectsTableName() const;
    void setObjectsTableName(const QString &value);
    QString getReferenceTableName() const;
    void setReferenceTableName(const QString &value);

    // parent getters and setters
    BaseHeapItem *getParent() const;
    void setParent(BaseHeapItem *value);
    bool getIsSnapshot() const;
    void setIsSnapshot(bool value);
    QString getOriginalObjectsTableName() const;
    void setOriginalObjectsTableName(const QString &value);
    virtual BaseHeapItem *getObjectParents(BaseHeapItem *childItem);
};

} // namespace RBKit

#endif // RBKIT_ABSTRACTHEAPITEM_H
