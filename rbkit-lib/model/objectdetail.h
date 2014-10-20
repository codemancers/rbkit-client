#ifndef OBJECTDETAIL_H
#define OBJECTDETAIL_H

#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QVariant>

namespace RBKit
{
    // https://github.com/code-mancers/rbkit/blob/47b461/ext/rbkit_event_packer.h#L6
    enum ObjectField
    {
        OfObjectId    = 3,
        OfClassName   = 4,
        OfReferences  = 5,
        OfFile        = 6,
        OfLine        = 7,
        OfSize        = 8
    };


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
}

Q_DECLARE_METATYPE(RBKit::ObjectDetail)

#endif // OBJECTDETAIL_H
