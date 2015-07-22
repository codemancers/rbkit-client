#include "cpunode.h"

CpuNode::CpuNode(QString methodName,
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

QDebug &operator<<(QDebug &stream, const CpuNode &myclass) {
    stream << "methodName :" << myclass.methodName;
    stream << "label :" << myclass.label;
    stream << "filename :" << myclass.filename;
    stream << "thread_id :" << myclass.thread_id;
    stream << "line_no :" << myclass.line_no;
    stream << "singleton_method :" << myclass.singleton_method;

    return stream;
}

void CpuNode::updateCalls(CpuNode &CpuNode) {
    this->calls.push_back(&CpuNode);
}

void CpuNode::updateCalledBy(CpuNode &CpuNode) {
    this->calledBy.push_back(&CpuNode);
}

QList<CpuNode *> CpuNode::getCalledBy() {
    return this->calledBy;
}

QList<CpuNode *> CpuNode::getCalls() {
    return this->calls;
}

QString CpuNode::getMethodName() {
    return this->methodName;
}

void CpuNode::updateData(QString methodName,
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

bool CpuNode::existInCalls(CpuNode* method) {
    if(this->calls.indexOf(method) == -1) {
        return false;
    } else {
        return true;
    }
}

bool CpuNode::existInCalledBy(CpuNode* method) {
    if(this->calledBy.indexOf(method) == -1) {
        return false;
    } else {
        return true;
    }
}
