#include "cpustorage.h"
#include <QVariant>
#include <QDebug>
#include "cpumapping.h"

RBKit::CpuStorage::CpuStorage()
{

}

void RBKit::CpuStorage::addNewNode(QMap<int, QVariant> data)
{
    //qDebug() << data;
    auto methodName = data[RBKit::CeMethodName].toString();

    RBKit::CpuNodePtr newNode( new RBKit::CpuNode(methodName,
                data[RBKit::CeLabel].toString(),
                data[RBKit::CeFile].toString(),
                data[RBKit::CeThreadId].toString(),
                data[RBKit::CeLine].toDouble(),
                data[RBKit::CeSingletonMethod].toInt())
            );

    Q_ASSERT(newNode != NULL);

    CpuStorage::updateNewNodeLocation(methodName, newNode);

    // increment total count
    newNode->incrementTotalCount();
    qDebug() << "----------incrementing total count------------";

    //add to current frame stack
    if(this->currentStack.empty()) {
        this->currentStack.push_back(data[RBKit::CeMethodName].toString());
    } else {
        QString currentTop = this->currentStack.back();
        this->nodes[currentTop]->updateCalls(newNode);
        newNode->updateCalledBy(this->nodes[currentTop]);

        this->currentStack.push_back(data[RBKit::CeMethodName].toString());
    }
}


// added newly created node & its pointer to the `nodes` hash
void RBKit::CpuStorage::updateNewNodeLocation(QString methodName, RBKit::CpuNodePtr location)
{
    this->nodes[methodName] = location;
}

bool RBKit::CpuStorage::exists(QVariant name)
{
    QString methodName = name.toString();
    QHash<QString, RBKit::CpuNodePtr>::iterator iter = this->nodes.find(methodName);
        if(iter == this->nodes.end()) {
            qDebug() << methodName << " does not exists";
            return false;
        } else {
            qDebug() << methodName << " exists";
            return true;
        }
}

unsigned long long RBKit::CpuStorage::getSampleCount()
{
    return this->sample_count;
}

void RBKit::CpuStorage::traverseNodes()
{
    for(QHash<QString, RBKit::CpuNodePtr>::iterator i = this->nodes.begin(); i != this->nodes.end(); i++) {
        qDebug() << "" << *i.value();
    }
}

void RBKit::CpuStorage::clearFrameStack()
{
    qDebug() << "Reset Frame Stack";
    this->currentStack.clear();
}

void RBKit::CpuStorage::traverseFlatProfile(QStandardItem &fgRootNode)
{
    int indent;
    char space = ' ';
    for(QHash<QString, RBKit::CpuNodePtr>::iterator node = this->nodes.begin(); node != this->nodes.end(); node++) {
        //qDebug() << "\n=====================\n";
        //qDebug() << "\n" + node.value()->getMethodName();

        QList<QStandardItem*> topLevelMethod = prepareRow(node.value()->getMethodName(),
                                                            node.value()->getSelfCount(),
                                                            node.value()->getTotalCount());
        fgRootNode.appendRow(topLevelMethod);

        QList<RBKit::CpuNodePtr> calledBy = node.value()->getCalledBy();
        indent=4;
        foreach(RBKit::CpuNodePtr node, calledBy) {
            //qDebug() << QString(indent, space) + node->getMethodName();

            QList<QStandardItem*> innerMethod = prepareRow(node->getMethodName(),
                                                           node->getSelfCount(),
                                                           node->getTotalCount());
            topLevelMethod.first()->appendRow(innerMethod);
        }
        //qDebug() << "\n=====================\n";
        //qDebug() << calledBy.size();
    }
}

void RBKit::CpuStorage::updateExistingMethod(QMap<int, QVariant> data) {

    QString methodName = data[RBKit::CeMethodName].toString();

    if(this->currentStack.empty()) {
        this->currentStack.push_back(methodName);
    } else {
        RBKit::CpuNodePtr existingNode = this->nodes[methodName];
        QString currentTop = this->currentStack.back();

        if(!this->nodes[currentTop]->existInCalls(existingNode)) {
            this->nodes[currentTop]->updateCalls(existingNode);
        }

        if(!this->nodes[data[RBKit::CeMethodName].toString()]->existInCalledBy(this->nodes[currentTop])) {
            existingNode->updateCalledBy(this->nodes[currentTop]);
        }

        existingNode->updateData(data[RBKit::CeMethodName].toString(),
                data[RBKit::CeLabel].toString(),
                data[RBKit::CeFile].toString(),
                data[RBKit::CeThreadId].toString(),
                data[RBKit::CeLine].toInt(),
                data[RBKit::CeSingletonMethod].toInt());
    }

    //increment the method's total count
    this->nodes[methodName]->incrementTotalCount();
    qDebug() << "--------updating total count---------";
}

void RBKit::CpuStorage::traverseCallGraph(RBKit::CpuNodePtr startingNode,
                                          QStandardItem &cgRootNode,
                                          QList<QStandardItem *> *parent=NULL)
{
    char space = ' ';
    QString methodName = startingNode->getMethodName();

    if(!this->notReached.contains(methodName)) {
        //qDebug() << QString(indent, space) + methodName;

        QList<QStandardItem*> currentMethod = prepareRow(methodName,
                                                         startingNode.data()->getSelfCount(),
                                                         startingNode.data()->getTotalCount());
        if(parent == NULL) {
            cgRootNode.appendRow(currentMethod);
        } else {
            parent->first()->appendRow(currentMethod);
        }
        return;

    } else {
        //qDebug() << QString(indent, space) + methodName;

        QList<QStandardItem*> currentMethod = prepareRow(methodName,
                                                         startingNode.data()->getSelfCount(),
                                                         startingNode.data()->getTotalCount());
        if(parent == NULL) {
            cgRootNode.appendRow(currentMethod);
        } else {
            parent->first()->appendRow(currentMethod);
        }

        this->notReached.removeOne(startingNode->getMethodName());

        foreach(RBKit::CpuNodePtr node, startingNode->getCalls()) {
            //qDebug() << QString(indent,space) + node->getMethodName();
            this->traverseCallGraph(node, cgRootNode, &currentMethod);
        }
    }


}

QHash<QString, RBKit::CpuNodePtr> RBKit::CpuStorage::getNodes()
{
    return this->nodes;
}

void RBKit::CpuStorage::handleCallGraph(QStandardItem &cgRootNode)
{
    this->notReached = this->nodes.keys();
    //qDebug() << notReached;
    while(!this->notReached.empty()) {
        CpuStorage::traverseCallGraph(this->nodes[this->notReached.front()], cgRootNode);
    }
}

RBKit::CpuStoragePtr RBKit::CpuStorage::getStorage()
{
    static RBKit::CpuStoragePtr store(new RBKit::CpuStorage());
    return store;
}

void RBKit::CpuStorage::updateSelfCount()
{
    if(!this->currentStack.isEmpty()) {
        qDebug() << "-------updating self count--------";
        QString lastMethod = this->currentStack.back();
        nodes[lastMethod]->incrementSelfCount();
    }
}

QList<QStandardItem*> RBKit::CpuStorage::prepareRow(QString methodName, int selfCount, int totalCount)
{
    QList<QStandardItem*> row;
    double selfTime = (selfCount * 100.0) / this->sample_count;
    double totalTime = (totalCount * 100.0) / this->sample_count;

    QStandardItem *selfPercent = new QStandardItem(QString::number(selfTime, 'f', 2));
    QStandardItem *totalPercent = new QStandardItem(QString::number(totalTime, 'f', 2));

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
    qDebug() << "^^^^^^^^creating flat graph model^^^^^^^^^^^";
    QStandardItem *fgRootNode = flatGraphModel->invisibleRootItem();
    traverseFlatProfile(*fgRootNode);
}
