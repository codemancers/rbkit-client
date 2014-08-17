#ifndef OBJECTDETAIL_H
#define OBJECTDETAIL_H

#include <QList>
#include <QString>
#include <QVariant>

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
        void addReference(quint64 reference);
        void updateGeneration();
    };
}

Q_DECLARE_METATYPE(RBKit::ObjectDetail)

#endif // OBJECTDETAIL_H
