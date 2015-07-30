#include "cpustorage.h"
#include <QVariant>
#include <QDebug>
#include "cpumapping.h"

void RBKit::CpuStorage::addNewNode(QMap<int, QVariant> data)
{
    //qDebug() << data;
    auto methodName = data[RBKit::ECeMethodName].toString();

    RBKit::CpuNodePtr newNode( new RBKit::CpuNode(methodName,
                data[RBKit::ECeLabel].toString(),
                data[RBKit::ECeFile].toString(),
                data[RBKit::ECeThreadId].toString(),
                data[RBKit::ECeLine].toDouble(),
                data[RBKit::ECeSingletonMethod].toInt())
            );

    Q_ASSERT(newNode != NULL);

    CpuStorage::updateNewNodeLocation(methodName, newNode);

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

void RBKit::CpuStorage::updateNewNodeLocation(QString methodName, RBKit::CpuNodePtr location)
{
    nodes[methodName] = location;
}

bool RBKit::CpuStorage::exists(QVariant name)
{
    QString methodName = name.toString();
    QHash<QString, RBKit::CpuNodePtr>::iterator iter = nodes.find(methodName);
        if(iter == nodes.end()) {
            qDebug() << methodName << " does not exists";
            return false;
        } else {
            qDebug() << methodName << " exists";
            return true;
        }
}

int RBKit::CpuStorage::getSampleCount()
{
    return sample_count;
}

void RBKit::CpuStorage::traverseNodes()
{
    for(QHash<QString, RBKit::CpuNodePtr>::iterator i = nodes.begin(); i != nodes.end(); i++) {
        qDebug() << "" << *i.value();
    }
}

void RBKit::CpuStorage::clearFrameStack()
{
    qDebug() << "Reset Frame Stack";
    currentStack.clear();
}

void RBKit::CpuStorage::traverseFlatProfile()
{
    int indent;
    char space = ' ';
    for(QHash<QString, RBKit::CpuNodePtr>::iterator node = nodes.begin(); node != nodes.end(); node++) {
        //qDebug() << "\n=====================\n";
        qDebug() << "\n" + node.value()->getMethodName();

        QStandardItem *topLevelMethod = new QStandardItem(node.value()->getMethodName());
        rootNode->appendRow(topLevelMethod);

        QList<RBKit::CpuNodePtr> calledBy = node.value()->getCalledBy();
        indent=4;
        foreach(RBKit::CpuNodePtr node, calledBy) {
            qDebug() << QString(indent, space) + node->getMethodName();

            QStandardItem *innerMethod = new QStandardItem(node->getMethodName());
            topLevelMethod->appendRow(innerMethod);
            //indent+=4;
        }
        //qDebug() << "\n=====================\n";
        //qDebug() << calledBy.size();
    }


    //createModel();
    emit updateTreeModel(standardModel);
}

void RBKit::CpuStorage::updateExistingMethod(QMap<int, QVariant> data) {
    if(currentStack.empty()) {
        currentStack.push_back(data[RBKit::ECeMethodName].toString());
    } else {
        RBKit::CpuNodePtr existingNode = nodes[data[RBKit::ECeMethodName].toString()];
        QString currentTop = currentStack.back();

        if(!nodes[currentTop]->existInCalls(existingNode)) {
            nodes[currentTop]->updateCalls(existingNode);
        }

        if(!nodes[data[RBKit::ECeMethodName].toString()]->existInCalledBy(nodes[currentTop])) {
            existingNode->updateCalledBy(nodes[currentTop]);
        }

        existingNode->updateData(data[RBKit::ECeMethodName].toString(),
                data[RBKit::ECeLabel].toString(),
                data[RBKit::ECeFile].toString(),
                data[RBKit::ECeThreadId].toString(),
                data[RBKit::ECeLine].toInt(),
                data[RBKit::ECeSingletonMethod].toInt());
    }
}

void RBKit::CpuStorage::traverseCallGraph(RBKit::CpuNodePtr startingNode, int indent)
{
    char space = ' ';
    QString methodName = startingNode->getMethodName();

    if(!notReached.contains(methodName)) {
        qDebug() << QString(indent, space) + methodName;
        return;
    } else {
        qDebug() << QString(indent, space) + methodName;
        notReached.removeOne(startingNode->getMethodName());

        foreach(RBKit::CpuNodePtr node, startingNode->getCalls()) {
            //qDebug() << QString(indent,space) + node->getMethodName();
            traverseCallGraph(node, indent+4);
        }
    }
}

QHash<QString, RBKit::CpuNodePtr> RBKit::CpuStorage::getNodes()
{
    return nodes;
}

void RBKit::CpuStorage::handleCallGraph()
{
    notReached = nodes.keys();
    //qDebug() << notReached;
    while(!notReached.empty()) {
        CpuStorage::traverseCallGraph(nodes[notReached.front()]);
    }
}


void RBKit::CpuStorage::stopCpuProfiling()
{
    qDebug() << "Profiling stopped";
}


RBKit::CpuStoragePtr RBKit::CpuStorage::getStorage()
{
    static RBKit::CpuStoragePtr store(new RBKit::CpuStorage());
    return store;
}

void RBKit::CpuStorage::changeToFlatProfile()
{
    qDebug() << "got signal to change to flat profile";
    traverseFlatProfile();
}

void RBKit::CpuStorage::changeToCallGraph()
{
    qDebug() << "got signal to change to call graph";
    handleCallGraph();
}
