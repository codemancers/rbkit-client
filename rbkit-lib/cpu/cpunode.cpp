#include "cpunode.h"

Node::Node(QString methodName,
           QString label,
           QString filename,
           QString thread_id,
           int line_no,
           int singleton_method)
{
    this->methodName = methodName;
    this->label = label;
    this->filename = filename;
    this->thread_id = thread_id;
    this->line_no = line_no;
    this->singleton_method = singleton_method;
}

QDebug &operator<<(QDebug &stream, const Node &myclass) {
    stream << "methodName :" << myclass.methodName;
    stream << "label :" << myclass.label;
    stream << "filename :" << myclass.filename;
    stream << "thread_id :" << myclass.thread_id;
    stream << "line_no :" << myclass.line_no;
    stream << "singleton_method :" << myclass.singleton_method;

    return stream;
}

void Node::updateCalls(Node *node) {
    this->calls.push_back(node);
}

void Node::updateCalledBy(Node *node) {
    this->calledBy.push_back(node);
}

std::vector<Node *> Node::getCalledBy() {
    return this->calledBy;
}

std::vector<Node *> Node::getCalls() {
    return this->calls;
}

QString Node::getMethodName() {
    return this->methodName;
}

void Node::updateData(QString methodName,
                      QString label,
                      QString filename,
                      QString thread_id,
                      int line_no,
                      int singleton_method) {
    QString str = methodName + label + filename + thread_id;
    int a = line_no + singleton_method;
    str.toUpper();
    a+10;

}
