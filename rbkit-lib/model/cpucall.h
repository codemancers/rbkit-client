#ifndef CPUCALL_H
#define CPUCALL_H
#include <QSharedPointer>
#include "cpunode.h"

class CpuCall
{
public:
    RBKit::CpuNodePtr ptr;
    CpuCall();
};

typedef QSharedPointer<CpuCall> CpuCallPtr;

#endif // CPUCALL_H
