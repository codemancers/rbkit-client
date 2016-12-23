#include "cpunode.h"

RBKit::CpuNode::CpuNode(QString methodName,
           QString label,
           QString filename,
           QString threadId,
           int lineNo,
           int singletonMethod) : methodName(methodName),
                                    label(label),
                                    filename(filename),
                                    threadId(threadId),
                                    lineNo(lineNo),
                                    singletonMethod(singletonMethod)
{
    this->selfCount=0;
    this->totalCount=0;
}

void RBKit::CpuNode::updateCalls(CpuNodePtr cpuNode)
{
    this->calls.push_back(cpuNode);
}

void RBKit::CpuNode::updateCalledBy(CpuNodePtr cpuNode)
{
    this->calledBy.push_back(cpuNode);
}

QList<RBKit::CpuNodePtr> RBKit::CpuNode::getCalledBy()
{
    return this->calledBy;
}

QList<RBKit::CpuNodePtr> RBKit::CpuNode::getCalls()
{
    return this->calls;
}

QString RBKit::CpuNode::getMethodName()
{
    return this->methodName;
}


//just a placeholder for now
//will be useful for updating
//reference count and other related
//things later on
void RBKit::CpuNode::updateData(QString methodName,
                      QString label,
                      QString filename,
                      QString thread_id,
                      int line_no,
                      int singleton_method)
{
    auto str = methodName + label + filename + thread_id;
    int a = line_no + singleton_method;
    str.toUpper();
    a+10;

}

bool RBKit::CpuNode::existInCalls(CpuNodePtr method)
{
    return this->calls.indexOf(method) != -1;
}

bool RBKit::CpuNode::existInCalledBy(CpuNodePtr method)
{
    return this->calledBy.indexOf(method) != -1;
}

void RBKit::CpuNode::incrementSelfCount()
{
    this->selfCount++;
}

void RBKit::CpuNode::incrementTotalCount()
{
    this->totalCount++;
}

unsigned long long RBKit::CpuNode::getTotalCount()
{
    return this->totalCount;
}

unsigned long long RBKit::CpuNode::getSelfCount()
{
    return this->selfCount;
}
