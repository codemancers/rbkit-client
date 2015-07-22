#include <QString>

#ifndef MAPPING
#define MAPPING

namespace RBKit {
    enum CpuEvent {
        CeFile              = 6,
        CeLine              = 7,
        CeMethodName        = 12,
        CeLabel             = 13,
        CeSingletonMethod   = 14,
        CeThreadId          = 15
    };
}

#endif // MAPPING

