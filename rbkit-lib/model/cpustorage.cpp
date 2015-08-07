#include "cpustorage.h"
#include <QVariant>
#include <QDebug>
#include "cpumapping.h"

RBKit::CpuStorage::CpuStorage()
{
    sample_count = 0;
}

void RBKit::CpuStorage::addNewNode(QMap<int, QVariant> data)
{
    auto methodName = data[RBKit::ECeMethodName].toString();

    if (!currentStack.isEmpty()) {
        QString prefix = currentStack.back();
        methodName = prefix + ":" + methodName;
    }

    RBKit::CpuNodePtr newNode( new RBKit::CpuNode(methodName,
                data[RBKit::ECeLabel].toString(),
                data[RBKit::ECeFile].toString(),
                data[RBKit::ECeThreadId].toString(),
                data[RBKit::ECeLine].toDouble(),
                data[RBKit::ECeSingletonMethod].toInt())
            );

    Q_ASSERT(newNode != NULL);

    CpuStorage::updateNewNodeLocation(methodName, newNode);

    // increment total count
    newNode->incrementTotalCount();

    //add to current frame stack
    if(currentStack.empty()) {
        currentStack.push_back(methodName);
    } else {
        QString currentTop = currentStack.back();
        nodes[currentTop]->updateCalls(newNode);
        newNode->updateCalledBy(nodes[currentTop]);

        currentStack.push_back(methodName);
    }
}


// added newly created node & its pointer to the `nodes` hash
void RBKit::CpuStorage::updateNewNodeLocation(QString methodName, RBKit::CpuNodePtr location)
{
    nodes[methodName] = location;
}

bool RBKit::CpuStorage::exists(QVariant name)
{
    QString methodName = name.toString();
    if (!currentStack.isEmpty()) {
        methodName = currentStack.back() + ":" + methodName;
    }

    QHash<QString, RBKit::CpuNodePtr>::iterator iter = nodes.find(methodName);
    if(iter == nodes.end()) {
        return false;
    } else {
        return true;
    }
}

unsigned long long RBKit::CpuStorage::getSampleCount()
{
    return sample_count;
}

void RBKit::CpuStorage::clearFrameStack()
{
    currentStack.clear();
}

void RBKit::CpuStorage::traverseFlatProfile(QStandardItem &fgRootNode)
{
    int indent;
    char space = ' ';
    for(QHash<QString, RBKit::CpuNodePtr>::iterator node = nodes.begin(); node != nodes.end(); node++) {

        QList<QStandardItem*> topLevelMethod = prepareRow(node.value()->getMethodName(),
                                                            node.value()->getSelfCount(),
                                                            node.value()->getTotalCount());
        fgRootNode.appendRow(topLevelMethod);

        QSet<CpuCalledByPtr> calledBy = node.value()->getCalledBy();
        indent=4;
        foreach(CpuCalledByPtr node, calledBy) {

            QList<QStandardItem*> innerMethod = prepareRow(node->getMethodName(),
                                                           node->getSelfCount(),
                                                           node->getTotalCount());
            topLevelMethod.first()->appendRow(innerMethod);
        }
    }
}

void RBKit::CpuStorage::updateExistingMethod(QMap<int, QVariant> data)
{
    QString methodName = data[RBKit::ECeMethodName].toString();

    if(currentStack.empty()) {
        currentStack.push_back(methodName);
    } else {
        RBKit::CpuNodePtr existingNode = nodes[methodName];
        QString currentTop = currentStack.back();

        nodes[currentTop]->updateCalls(existingNode);

        existingNode->updateCalledBy(nodes[currentTop]);

        existingNode->updateData(data[RBKit::ECeMethodName].toString(),
                data[RBKit::ECeLabel].toString(),
                data[RBKit::ECeFile].toString(),
                data[RBKit::ECeThreadId].toString(),
                data[RBKit::ECeLine].toInt(),
                data[RBKit::ECeSingletonMethod].toInt());
    }

    //increment the method's total count
    nodes[methodName]->incrementTotalCount();

    // increment the total count of all methods above the current method
    // int the current stack
    //for(int i = 0; i < currentStack.size() - 1; i++) {
    //  nodes[currentStack[currentStack.size() - 1]]->incrementTotalCount();
    //}
}

void RBKit::CpuStorage::traverseCallGraph(CpuCallPtr startingNode,
                                          QStandardItem &cgRootNode,
                                          QStandardItem *parent = NULL)
{
    char space = ' ';
    QString methodName = startingNode->getMethodName();

    if(!notReached.contains(methodName)) {

        QList<QStandardItem*> currentMethod = prepareRow(methodName,
                                                         startingNode.data()->getSelfCount(),
                                                         startingNode.data()->getTotalCount());
        if(parent == NULL) {
            cgRootNode.appendRow(currentMethod);
        } else {
            parent->appendRow(currentMethod);
        }
        return;

    } else {

        QList<QStandardItem*> currentMethod = prepareRow(methodName,
                                                         startingNode.data()->getSelfCount(),
                                                         startingNode.data()->getTotalCount());
        if(parent == NULL) {
            cgRootNode.appendRow(currentMethod);
        } else {
            parent->appendRow(currentMethod);
        }

        notReached.removeOne(startingNode->getMethodName());

        for (auto node : startingNode->getCalls()) {
            traverseCallGraph(node, cgRootNode, currentMethod.first());
        }
    }


}

QHash<QString, RBKit::CpuNodePtr> RBKit::CpuStorage::getNodes()
{
    return nodes;
}

void RBKit::CpuStorage::handleCallGraph(QStandardItem &cgRootNode)
{
    notReached = nodes.keys();
    while(!notReached.empty()) {
        CpuStorage::traverseCallGraph(nodes[notReached.front()], cgRootNode);
    }
}

RBKit::CpuStoragePtr RBKit::CpuStorage::getStorage()
{
    static RBKit::CpuStoragePtr store(new RBKit::CpuStorage());
    return store;
}

void RBKit::CpuStorage::updateSelfCount()
{
    if(!currentStack.isEmpty()) {
        QString lastMethod = currentStack.back();
        nodes[lastMethod]->incrementSelfCount();
    }
}

QList<QStandardItem*> RBKit::CpuStorage::prepareRow(QString methodName, int selfCount, int totalCount)
{
    QList<QStandardItem*> row;
    double selfTime = (selfCount * 100.0) / sample_count;
    double totalTime = (totalCount * 100.0) / sample_count;

    qDebug() << "selfTime ------------->" << selfCount;
    qDebug() << "totalTime -------------->" << totalCount;
    qDebug() << "sample count ------------->" << sample_count;

    QStandardItem *selfPercent = new QStandardItem(QString::number(selfTime, 'f', 2) + " %");
    QStandardItem *totalPercent = new QStandardItem(QString::number(totalTime, 'f', 2) + " %");

    // setting center alignments
    selfPercent->setTextAlignment(Qt::AlignHCenter);
    totalPercent->setTextAlignment(Qt::AlignHCenter);

    row << new QStandardItem(methodName);
    row << selfPercent;                                 //self time percentage
    row << totalPercent;                                //total time parcentage

    return row;
}

void RBKit::CpuStorage::fillCallGraphModel(QStandardItemModel *callGraphModel)
{
    QStandardItem *cgRootNode = callGraphModel->invisibleRootItem();
    handleCallGraph(*cgRootNode);
}

void RBKit::CpuStorage::fillFlatProfileModel(QStandardItemModel *flatGraphModel)
{
    QStandardItem *fgRootNode = flatGraphModel->invisibleRootItem();
    traverseFlatProfile(*fgRootNode);
}
