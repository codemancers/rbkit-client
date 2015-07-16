#include <QString>

#ifndef MAPPING
#define MAPPING

inline namespace MAPS {
    const QString event_type = "0",
            timestamp = "1",
            payload = "2",
            object_id = "3",
            class_name = "4",
            references = "5",
            file = "6",
            line = "7",
            size = "8",
            message_counter = "9",
            correlation_id = "10",
            complete_message_count = "11",
            method_name = "12",
            label = "13",
            singleton_method = "14",
            thread_id = "15";
}

#endif // MAPPING

