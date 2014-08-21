#ifndef OBJECTDETAIL_H
#define OBJECTDETAIL_H

#include <QList>
#include <QString>
#include <QVariant>
#include "rbevents.h"

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
    };
}

Q_DECLARE_METATYPE(RBKit::ObjectDetail)

#endif // OBJECTDETAIL_H
