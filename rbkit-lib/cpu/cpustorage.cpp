#include "cpustorage.h"
#include <QVariant>
#include <QDebug>
#include "cpumapping.h"

storage::storage()
{

}

void storage::addNewNode(QVariantMap data) {
    //qDebug() << data;
    Node *newNode = new Node(data[MAPS::method_name].toString(),
            data[MAPS::label].toString(),
            data[MAPS::file].toString(),
            data[MAPS::thread_id].toString(),
            data[MAPS::line].toDouble(),
            data[MAPS::singleton_method].toInt());

    storage::updateNewNodeLocation(data[MAPS::method_name].toString(), newNode);

    //add to current frame stack
    if(this->currentStack.empty()) {
        this->currentStack.push_back(data[MAPS::method_name].toString());
    } else {
        QString currentTop = this->currentStack.back();
        this->nodes[currentTop]->updateCalls(newNode);
        newNode->updateCalledBy(this->nodes[currentTop]);

        this->currentStack.push_back(data[MAPS::method_name].toString());
    }
}

void storage::updateNewNodeLocation(QString methodName, Node *location) {
    this->nodes[methodName] = location;
}

bool storage::exists(QString method_name) {
QHash<QString, Node*>::iterator iter = this->nodes.find(method_name);
    if(iter == this->nodes.end()) {
        //qDebug() << method_name << " does not exists";
        return false;
    } else {
        //qDebug() << method_name << " exists";
        return true;
    }
}

void storage::incrementSampleCount() {
    this->sample_count++;
}

int storage::getSampleCount() {
    return this->sample_count;
}

void storage::traverseNodes() {
    for(QHash<QString, Node*>::iterator i = this->nodes.begin(); i != this->nodes.end(); i++) {
        //qDebug() << "" << *i.value();
    }
}

void storage::clearFrameStack() {
    //qDebug() << "Reset Frame Stack";
    this->currentStack.clear();
}

void storage::traverseFlatProfile() {
    int indent;
    char space = ' ';
    for(QHash<QString, Node*>::iterator node = this->nodes.begin(); node != this->nodes.end(); node++) {
        "\n" + node.value()->getMethodName();

        std::vector<Node*> calledBy = node.value()->getCalledBy();
        indent=4;
        foreach(Node* node, calledBy) {
            QString(indent, space) + node->getMethodName();
            indent+=4;
        }
        calledBy.size();
    }
}

void storage::updateExistingMethod(QVariantMap data) {
    if(this->currentStack.empty()) {
        this->currentStack.push_back(data[MAPS::method_name].toString());
    } else {
        Node *newNode = this->nodes[data[MAPS::method_name].toString()];
        QString currentTop = this->currentStack.back();
        this->nodes[currentTop]->updateCalls(newNode);

        newNode->updateCalledBy(this->nodes[currentTop]);

        newNode->updateData(data[MAPS::method_name].toString(),
                data[MAPS::label].toString(),
                data[MAPS::file].toString(),
                data[MAPS::thread_id].toString(),
                data[MAPS::line].toInt(),
                data[MAPS::singleton_method].toInt());
    }
}

void storage::traverseCallGraph(Node *startingNode, int indent) {
    char space = ' ';
    QString methodName = startingNode->getMethodName();

    if(!this->notReached.contains(methodName)) {
        return;
    } else {
        QString(indent, space) + methodName;
        this->notReached.removeOne(startingNode->getMethodName());

        foreach(Node* node, startingNode->getCalls()) {
            QString(indent,space) + node->getMethodName();
            this->traverseCallGraph(node, indent+4);
        }
    }

}

QHash<QString, Node *> storage::getNodes() {
    return this->nodes;
}

void storage::handleCallGraph() {
    this->notReached = this->nodes.keys();
    while(!this->notReached.empty()) {
        storage::traverseCallGraph(this->nodes[this->notReached.front()]);
    }
}
