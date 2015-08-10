#include "cpunode.h"
#include "cpucall.h"
#include "cpucalledby.h"

RBKit::CpuNode::CpuNode(QString methodName,
                        QString methodGroup,
                        QString label,
                        QString filename,
                        QString threadId,
                        int lineNo,
                        int singletonMethod) : methodName(methodName),
                                            methodGroup(methodGroup),
                                            label(label),
                                            filename(filename),
                                            threadId(threadId),
                                            lineNo(lineNo),
                                            singletonMethod(singletonMethod),
                                            selfCount(0),
                                            totalCount(0) {}

void RBKit::CpuNode::updateCalls(CpuNodePtr cpuNode)
{
    CpuCallPtr temp(new CpuCall());
    temp->ptr = cpuNode;
    calls.insert(temp);
}

void RBKit::CpuNode::updateCalledBy(CpuNodePtr cpuNode)
{
    CpuCalledByPtr temp(new CpuCalledBy());
    temp->ptr = cpuNode;
    calledBy.insert(temp);
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
