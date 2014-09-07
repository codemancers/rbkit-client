#ifndef RBKIT_STRINGUTIL_H
#define RBKIT_STRINGUTIL_H

#include "msgpack.hpp"
#include <QObject>
#include <QCryptographicHash>
#include <QString>
#include <QDateTime>

namespace RBKit {
    namespace StringUtil {
        QString rawToQString(msgpack::object object);
        quint64 hextoInt(const QString &string);
        QString randomSHA();
    };
};

#endif // RBKIT_STRINGUTIL_H
