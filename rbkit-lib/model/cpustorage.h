#ifndef STORAGE_H
#define STORAGE_H
#include <QHash>
#include "cpunode.h"
#include <QVariant>
#include <vector>

namespace RBKit
{
    class CpuStorage
    {
        int sample_count;
        QHash<QString, RBKit::CpuNodePtr> nodes;
        QList<QString> notReached;


        void traverseCallGraph(RBKit::CpuNodePtr, int indent=1);
    public:
        QList<QString> currentStack;

        void addNewNode(QMap<int, QVariant>);
        void updateNewNodeLocation(QString methodName, RBKit::CpuNodePtr);
        bool exists(QVariant name);
        int getSampleCount();
        void traverseNodes();
        void clearFrameStack();
        void updateExistingMethod(QMap<int, QVariant>);

        void traverseFlatProfile();
        void handleCallGraph();
        QHash<QString, RBKit::CpuNodePtr> getNodes();

        inline void incrementSampleCount() {
            this->sample_count++;
        }
    };

    typedef QSharedPointer<CpuStorage> CpuStoragePtr;
}

#endif // STORAGE_H
