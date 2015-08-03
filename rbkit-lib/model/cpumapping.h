#include <QString>

#ifndef MAPPING
#define MAPPING

namespace RBKit
{
    enum CpuEvent
    {
        ECeFile              = 6,
        ECeLine              = 7,
        ECeMethodName        = 12,
        ECeLabel             = 13,
        ECeSingletonMethod   = 14,
        ECeThreadId          = 15
    };
}

#endif // MAPPING

