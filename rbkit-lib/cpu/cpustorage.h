#ifndef STORAGE_H
#define STORAGE_H
#include <QHash>
#include "cpunode.h"
#include <QVariant>
#include <vector>

class CpuStorage
{
    int sample_count;
    QHash<QString, CpuNode*> nodes;
    QList<QString> notReached;


    void traverseCallGraph(CpuNode*, int indent=1);
public:
    QList<QString> currentStack;

    void addNewNode(QMap<int, QVariant>);
    void updateNewNodeLocation(QString methodName, CpuNode*);
    bool exists(QVariant name);
    int getSampleCount();
    void traverseNodes();
    void clearFrameStack();
    void updateExistingMethod(QMap<int, QVariant>);

    void traverseFlatProfile();
    void handleCallGraph();
    QHash<QString, CpuNode*> getNodes();

    inline void incrementSampleCount() {
        this->sample_count++;
    }
};

#endif // STORAGE_H
