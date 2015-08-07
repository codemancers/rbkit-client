#ifndef CPUCALLEDBY_H
#define CPUCALLEDBY_H
#include <QSharedPointer>
#include "cpunode.h"

class CpuCalledBy
{
public:
    RBKit::CpuNodePtr ptr;
    CpuCalledBy();
};

typedef QSharedPointer<CpuCalledBy> CpuCalledByPtr;

#endif // CPUCALLEDBY_H
