#include "cpunode.h"

/*CpuNode::CpuNode(QString methodName,
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
}*/

CpuNode::CpuNode(QString methodName,
           QString label,
           QString filename,
           QString threadId,
           int lineNo,
           int singletonMethod) : methodName(methodName),
                                    label(label),
                                    filename(filename),
                                    threadId(threadId),
                                    lineNo(lineNo),
                                    singletonMethod(singletonMethod) {}

QDebug &operator<<(QDebug &stream, const CpuNode &myclass) {
    stream << "methodName :" << myclass.methodName;
    stream << "label :" << myclass.label;
    stream << "filename :" << myclass.filename;
    stream << "thread_id :" << myclass.threadId;
    stream << "line_no :" << myclass.lineNo;
    stream << "singleton_method :" << myclass.singletonMethod;

    return stream;
}

void CpuNode::updateCalls(CpuNodePtr cpuNode) {
    this->calls.push_back(cpuNode);
}

void CpuNode::updateCalledBy(CpuNodePtr cpuNode) {
    this->calledBy.push_back(cpuNode);
}

QList<CpuNodePtr> CpuNode::getCalledBy() {
    return this->calledBy;
}

QList<CpuNodePtr> CpuNode::getCalls() {
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

bool CpuNode::existInCalls(CpuNodePtr method) {
    return this->calls.indexOf(method) != -1;
}

bool CpuNode::existInCalledBy(CpuNodePtr method) {
    return this->calledBy.indexOf(method) != -1;
}
