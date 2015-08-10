#ifndef CPUCALL_H
#define CPUCALL_H

#include <QSharedPointer>


namespace RBKit
{
    class CpuNode;

    class CpuCall
    {
    public:
        QSharedPointer<RBKit::CpuNode> ptr;
        CpuCall();

        friend bool &operator==(QSharedPointer<CpuCall> &ptr1, QSharedPointer<CpuCall> &ptr2)
        {
            QString method1 = ptr1->ptr->getMethodName();
            QString method2 = ptr2->ptr->getMethodName();

            if (method1 == method2) {
                return true;
            } else {
                return false;
            }
        }
    };

    typedef QSharedPointer<CpuCall> CpuCallPtr;
}

#endif // CPUCALL_H
