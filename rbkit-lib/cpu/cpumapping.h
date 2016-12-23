#include <QString>

#ifndef MAPPING
#define MAPPING

namespace RBKit {
    enum Map {
        eventType,
        timestamp,
        payload,
        objectId,
        className,
        references,
        file,
        line,
        size,
        messageCounter,
        correlationId,
        completeMessageCount,
        methodName,
        label,
        singletonMethod,
        threadId
    };
}

#endif // MAPPING

