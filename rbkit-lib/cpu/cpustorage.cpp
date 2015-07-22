#include "cpustorage.h"
#include <QVariant>
#include <QDebug>
#include "cpumapping.h"

void CpuStorage::addNewNode(QMap<int, QVariant> data) {
    //qDebug() << data;
    CpuNode *newNode = new CpuNode(data[RBKit::methodName].toString(),
            data[RBKit::label].toString(),
            data[RBKit::file].toString(),
            data[RBKit::threadId].toString(),
            data[RBKit::line].toDouble(),
            data[RBKit::singletonMethod].toInt());

    CpuStorage::updateNewNodeLocation(data[RBKit::methodName].toString(), newNode);

    //add to current frame stack
    if(this->currentStack.empty()) {
        this->currentStack.push_back(data[RBKit::methodName].toString());
    } else {
        QString currentTop = this->currentStack.back();
        this->nodes[currentTop]->updateCalls(*newNode);
        newNode->updateCalledBy(*this->nodes[currentTop]);

        this->currentStack.push_back(data[RBKit::methodName].toString());
    }
}

void CpuStorage::updateNewNodeLocation(QString methodName, CpuNode *location) {
    this->nodes[methodName] = location;
}

bool CpuStorage::exists(QVariant name) {
    QString methodName = name.toString();
    QHash<QString, CpuNode*>::iterator iter = this->nodes.find(methodName);
        if(iter == this->nodes.end()) {
            qDebug() << methodName << " does not exists";
            return false;
        } else {
            qDebug() << methodName << " exists";
            return true;
        }
}

int CpuStorage::getSampleCount() {
    return this->sample_count;
}

void CpuStorage::traverseNodes() {
    for(QHash<QString, CpuNode*>::iterator i = this->nodes.begin(); i != this->nodes.end(); i++) {
        qDebug() << "" << *i.value();
    }
}

void CpuStorage::clearFrameStack() {
    qDebug() << "Reset Frame Stack";
    this->currentStack.clear();
}

void CpuStorage::traverseFlatProfile() {
    int indent;
    char space = ' ';
    for(QHash<QString, CpuNode*>::iterator node = this->nodes.begin(); node != this->nodes.end(); node++) {
        //qDebug() << "\n=====================\n";
        qDebug() << "\n" + node.value()->getMethodName();

        QList<CpuNode*> calledBy = node.value()->getCalledBy();
        indent=4;
        foreach(CpuNode* node, calledBy) {
            qDebug() << QString(indent, space) + node->getMethodName();
            //indent+=4;
        }
        //qDebug() << "\n=====================\n";
        //qDebug() << calledBy.size();
    }
}

void CpuStorage::updateExistingMethod(QMap<int, QVariant> data) {
    if(this->currentStack.empty()) {
        this->currentStack.push_back(data[RBKit::methodName].toString());
    } else {
        CpuNode *existingNode = this->nodes[data[RBKit::methodName].toString()];
        QString currentTop = this->currentStack.back();

        if(!this->nodes[currentTop]->existInCalls(existingNode)) {
            this->nodes[currentTop]->updateCalls(*existingNode);
        }

        if(!this->nodes[data[RBKit::methodName].toString()]->existInCalledBy(this->nodes[currentTop])) {
            existingNode->updateCalledBy(*this->nodes[currentTop]);
        }

        existingNode->updateData(data[RBKit::methodName].toString(),
                data[RBKit::label].toString(),
                data[RBKit::file].toString(),
                data[RBKit::threadId].toString(),
                data[RBKit::line].toInt(),
                data[RBKit::singletonMethod].toInt());
    }
}

void CpuStorage::traverseCallGraph(CpuNode *startingNode, int indent) {
    char space = ' ';
    QString methodName = startingNode->getMethodName();

    if(!this->notReached.contains(methodName)) {
        qDebug() << QString(indent, space) + methodName;
        return;
    } else {
        qDebug() << QString(indent, space) + methodName;        
        this->notReached.removeOne(startingNode->getMethodName());

        foreach(CpuNode* node, startingNode->getCalls()) {
            //qDebug() << QString(indent,space) + node->getMethodName();
            this->traverseCallGraph(node, indent+4);
        }
    }
}

QHash<QString, CpuNode *> CpuStorage::getNodes() {
    return this->nodes;
}

void CpuStorage::handleCallGraph() {
    this->notReached = this->nodes.keys();
    //qDebug() << notReached;
    while(!this->notReached.empty()) {
        CpuStorage::traverseCallGraph(this->nodes[this->notReached.front()]);
    }
}
