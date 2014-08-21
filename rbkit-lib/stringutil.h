#ifndef RBKIT_STRINGUTIL_H
#define RBKIT_STRINGUTIL_H

#include "msgpack.hpp"
#include <QObject>

namespace RBKit {
    namespace StringUtil {
        QString rawToQString(msgpack::object object);
        quint64 hextoInt(const QString &string);
    };
};

#endif // RBKIT_STRINGUTIL_H
