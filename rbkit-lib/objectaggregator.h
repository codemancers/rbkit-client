#ifndef RBKIT_OBJECTAGGREGATOR_H
#define RBKIT_OBJECTAGGREGATOR_H

#include "objectdetail.h"
#include <QHash>


namespace RBKit
{
    class ObjectAggregator : public QObject
    {
        Q_OBJECT;

    public:
        ObjectAggregator(QObject* parent = NULL);
        ~ObjectAggregator() {};

    public:
        void objCreated(RBKit::ObjectDetailPtr objectPtr);
        void objDeleted(quint64 key);
        void updateFromSnapshot(const QList<ObjectDetailPtr>&);
        void onGcStats(const QVariantMap& map);

        QHash<QString, double> liveStats() const;

    public:
        // mapping between object class and its count
        QHash<QString, quint32> typeToCount;

        // mapping between id and class name
        QHash<quint64, QString> idToName;

        quint64 totalObjects;
        quint64 totalHeapSize;
        quint64 totalMemSize;
    };
}


#endif // RBKIT_OBJECTAGGREGATOR_H
