#ifndef STORAGE_H
#define STORAGE_H
#include <QHash>
#include "cpunode.h"
#include <QVariant>
#include <vector>

class storage
{
    int sample_count;
    QHash<QString, Node*> nodes;
    QList<QString> notReached;
public:
    std::vector<QString> currentStack;

    storage();

    void addNewNode(QVariantMap);
    void updateNewNodeLocation(QString methodName, Node*);
    bool exists(QString);
    void incrementSampleCount();
    int getSampleCount();
    void traverseNodes();
    void clearFrameStack();
    void updateExistingMethod(QVariantMap);

    void traverseFlatProfile();
    void traverseCallGraph(Node*, int indent=1);
    void handleCallGraph();
    QHash<QString, Node*> getNodes();
};

#endif // STORAGE_H
