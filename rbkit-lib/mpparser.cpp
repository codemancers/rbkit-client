#include "mpparser.h"


// convert object to QString
QString& operator>>(msgpack::object obj, QString& str)
{
    if (obj.type != msgpack::type::RAW) { throw msgpack::type_error(); }

    msgpack::object_raw raw = obj.via.raw;
    str = QString::fromUtf8(raw.ptr, raw.size);
    return str;
}


// convert any value to QVariant.
QVariant& operator>>(msgpack::object obj, QVariant& var)
{
    switch (obj.type) {
    case msgpack::type::ARRAY :
        var = obj.as< QList<QVariant> >();
        break;
    case msgpack::type::MAP :
        var = obj.as< QMap<QString, QVariant> >();
        break;

    case msgpack::type::RAW :
        var = obj.as<QString>();
        break;
    case msgpack::type::DOUBLE :
        var = obj.as<double>();
        break;
    case msgpack::type::POSITIVE_INTEGER :
        var = obj.as<unsigned long long>();
        break;
    case msgpack::type::NIL :
        var = QVariant("");
        break;

    default:
        throw "unknown object type";
    }

    return var;
}


// convert a payload into objectdetail.
RBKit::ObjectDetail& operator>>(msgpack::object obj, RBKit::ObjectDetail& object)
{
    if (obj.type != msgpack::type::MAP) { throw msgpack::type_error(); }

    msgpack::object_kv* list = obj.via.map.ptr;
    for (uint32_t iter = 0; iter != obj.via.map.size; ++iter) {
        auto val = list->val;

        if (msgpack::type::NIL == val.type) {
            continue;
        }

        auto key = list->key.as<unsigned int>();

        switch (key) {
        case RBKit::OfObjectId:
            object.objectId = val.as<unsigned long long>();
            break;
        case RBKit::OfFile:
            object.fileName = val.as<QString>();
            break;
        case RBKit::OfClassName:
            object.className = val.as<QString>();
            break;
        case RBKit::OfLine:
            object.lineNumber = val.as<unsigned long>();
            break;
        case RBKit::OfReferences:
            val >> object.references;
            break;
        case RBKit::OfSize:
            object.size = val.as<unsigned int>();
            break;
        default:
            Q_ASSERT(false);
        }
        ++list;
    }

    return object;
}


RBKit::ObjectDetailPtr& operator>>(msgpack::object obj, RBKit::ObjectDetailPtr& ptr)
{
    if (obj.type != msgpack::type::MAP) { throw msgpack::type_error(); }

    ptr.reset(new RBKit::ObjectDetail());
    obj >> *ptr;
    return ptr;
}
