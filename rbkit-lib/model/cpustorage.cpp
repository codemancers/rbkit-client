#include "cpustorage.h"
#include <QVariant>
#include <QDebug>
#include "cpumapping.h"

void CpuStorage::addNewNode(QMap<int, QVariant> data)
{
    //qDebug() << data;
    CpuNodePtr newNode( new CpuNode(data[RBKit::CeMethodName].toString(),
                data[RBKit::CeLabel].toString(),
                data[RBKit::CeFile].toString(),
                data[RBKit::CeThreadId].toString(),
                data[RBKit::CeLine].toDouble(),
                data[RBKit::CeSingletonMethod].toInt())
            );

    CpuStorage::updateNewNodeLocation(data[RBKit::CeMethodName].toString(), newNode);

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

void CpuStorage::updateNewNodeLocation(QString methodName, QSharedPointer<CpuNode> location)
{
    this->nodes[methodName] = location;
}

bool CpuStorage::exists(QVariant name)
{
    QString methodName = name.toString();
    QHash<QString, CpuNodePtr>::iterator iter = this->nodes.find(methodName);
        if(iter == this->nodes.end()) {
            qDebug() << methodName << " does not exists";
            return false;
        } else {
            qDebug() << methodName << " exists";
            return true;
        }
}

int CpuStorage::getSampleCount()
{
    return this->sample_count;
}

void CpuStorage::traverseNodes()
{
    for(QHash<QString, CpuNodePtr>::iterator i = this->nodes.begin(); i != this->nodes.end(); i++) {
        qDebug() << "" << *i.value();
    }
}

void CpuStorage::clearFrameStack()
{
    qDebug() << "Reset Frame Stack";
    this->currentStack.clear();
}

void CpuStorage::traverseFlatProfile()
{
    int indent;
    char space = ' ';
    for(QHash<QString, CpuNodePtr>::iterator node = this->nodes.begin(); node != this->nodes.end(); node++) {
        //qDebug() << "\n=====================\n";
        qDebug() << "\n" + node.value()->getMethodName();

        QList<CpuNodePtr> calledBy = node.value()->getCalledBy();
        indent=4;
        foreach(CpuNodePtr node, calledBy) {
            qDebug() << QString(indent, space) + node->getMethodName();
            //indent+=4;
        }
        //qDebug() << "\n=====================\n";
        //qDebug() << calledBy.size();
    }
}

void CpuStorage::updateExistingMethod(QMap<int, QVariant> data) {
    if(this->currentStack.empty()) {
        this->currentStack.push_back(data[RBKit::CeMethodName].toString());
    } else {
        CpuNodePtr existingNode = this->nodes[data[RBKit::CeMethodName].toString()];
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
}

void CpuStorage::traverseCallGraph(CpuNodePtr startingNode, int indent)
{
    char space = ' ';
    QString methodName = startingNode->getMethodName();

    if(!this->notReached.contains(methodName)) {
        qDebug() << QString(indent, space) + methodName;
        return;
    } else {
        qDebug() << QString(indent, space) + methodName;        
        this->notReached.removeOne(startingNode->getMethodName());

        foreach(CpuNodePtr node, startingNode->getCalls()) {
            //qDebug() << QString(indent,space) + node->getMethodName();
            this->traverseCallGraph(node, indent+4);
        }
    }
}

QHash<QString, CpuNodePtr> CpuStorage::getNodes()
{
    return this->nodes;
}

void CpuStorage::handleCallGraph()
{
    this->notReached = this->nodes.keys();
    //qDebug() << notReached;
    while(!this->notReached.empty()) {
        CpuStorage::traverseCallGraph(this->nodes[this->notReached.front()]);
    }
}
