#ifndef OBJECTDETAIL_H
#define OBJECTDETAIL_H

#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QVariant>
#include <QHash>


namespace RBKit {
    class ObjectDetail
    {
    public:
        ObjectDetail();
        ObjectDetail(QString className, quint64 objectId);
        ObjectDetail(const ObjectDetail&original);
        QString className;
        quint64 objectId;
        QString fileName;
        quint32 lineNumber;
        quint16 objectGeneration;
        QList<quint64> references;
        quint64 size;

        void addReference(quint64 reference);
        void addReferences(QList<QVariant> _references);
        void updateGeneration();
        QString getFileLine() const;
    };

    // typedef for the pointer.
    typedef QSharedPointer<ObjectDetail> ObjectDetailPtr;
    typedef QHash<quint64, RBKit::ObjectDetailPtr> QHashObjectIdToPtr;

    QList<ObjectDetailPtr> payloadToObjects(const QVariantList& list);
    ObjectDetailPtr payloadToObject(const QVariantMap& map);
}

Q_DECLARE_METATYPE(RBKit::ObjectDetail);
Q_DECLARE_METATYPE(RBKit::QHashObjectIdToPtr);


#endif // OBJECTDETAIL_H
