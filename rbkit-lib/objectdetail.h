#ifndef OBJECTDETAIL_H
#define OBJECTDETAIL_H

#include <QList>
#include <QString>

namespace RBKit {
    class ObjectDetail
    {
    public:
        ObjectDetail(QString className, quint64 objectId);
        QString className;
        quint64 objectId;
        QString fileName;
        quint32 lineNumber;
        QList<quint64> references;
        void addReference(quint64 reference);
    };
}
#endif // OBJECTDETAIL_H
