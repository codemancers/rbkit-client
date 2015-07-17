#include <QString>

#ifndef MAPPING
#define MAPPING

inline namespace MAPS {
    enum Map {
        event_type,
        timestamp,
        payload,
        object_id,
        class_name,
        references,
        file,
        line,
        size,
        message_counter,
        correlation_id,
        complete_message_count,
        method_name,
        label,
        singleton_method,
        thread_id
    };
}

#endif // MAPPING

