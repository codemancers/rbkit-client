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


    void traverseCallGraph(Node*, int indent=1);
public:
    std::vector<QString> currentStack;

    storage();

    void addNewNode(QMap<int, QVariant>);
    void updateNewNodeLocation(QString methodName, Node*);
    bool exists(QVariant name);
    void incrementSampleCount();
    int getSampleCount();
    void traverseNodes();
    void clearFrameStack();
    void updateExistingMethod(QMap<int, QVariant>);

    void traverseFlatProfile();
    void handleCallGraph();
    QHash<QString, Node*> getNodes();
};

#endif // STORAGE_H
