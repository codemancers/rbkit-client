#include "stringutil.h"

namespace RBKit {
namespace StringUtil {

QString rawToQString(msgpack::object object)
{
    msgpack::object_raw raw = object.via.raw;
    return QString::fromUtf8(raw.ptr, raw.size);
}

quint64 hextoInt(const QString &string)
{
    bool ok;
    quint64 hex = string.toULongLong(&ok, 16);
    if(ok) {
        return hex;
    } else {
        return 0;
    }
}


}
} // namespace RBKit
