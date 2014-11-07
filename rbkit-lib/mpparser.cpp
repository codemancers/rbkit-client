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

    auto map = obj.as< QMap<unsigned int, msgpack::object> >();

    if (! map[RBKit::OfObjectId].is_nil()) {
        object.objectId = map[RBKit::OfObjectId].as<unsigned long long>();
    }

    if (! map[RBKit::OfFile].is_nil()) {
        object.fileName = map[RBKit::OfFile].as<QString>();
    }

    if (! map[RBKit::OfClassName].is_nil()) {
        object.className = map[RBKit::OfClassName].as<QString>();
    }

    if (! map[RBKit::OfLine].is_nil()) {
        object.lineNumber = map[RBKit::OfLine].as<int>();
    }

    if (! map[RBKit::OfReferences].is_nil()) {
        object.addReferences(map[RBKit::OfReferences].as<QVariantList>());
    }

    if (! map[RBKit::OfSize].is_nil()) {
        object.size = map[RBKit::OfSize].as<int>();
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
