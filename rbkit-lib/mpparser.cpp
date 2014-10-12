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

    auto map = obj.as< QMap<QString, msgpack::object> >();

    if (! map["object_id"].is_nil()) {
        object.objectId = map["object_id"].as<unsigned long long>();
    }

    if (! map["file"].is_nil()) {
        object.fileName = map["file"].as<QString>();
    }

    if (! map["class_name"].is_nil()) {
        object.className = map["class_name"].as<QString>();
    }

    if (! map["line"].is_nil()) {
        object.lineNumber = map["line"].as<int>();
    }

    if (! map["references"].is_nil()) {
        object.addReferences(map["references"].as<QVariantList>());
    }

    if (! map["size"].is_nil()) {
        object.size = map["size"].as<int>();
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
