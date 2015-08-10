#ifndef CPUCALLEDBY_H
#define CPUCALLEDBY_H

#include <QSharedPointer>


namespace RBKit
{
    class CpuNode;

    class CpuCalledBy
    {
    public:
        QSharedPointer<RBKit::CpuNode> ptr;

        CpuCalledBy();
    };

    typedef QSharedPointer<CpuCalledBy> CpuCalledByPtr;
}

#endif // CPUCALLEDBY_H
