#ifndef RBKIT_MSGPACK_PARSER_H
#define RBKIT_MSGPACK_PARSER_H

#include <msgpack.hpp>

#include <QVariantMap>
#include <QString>
#include <QDebug>

#include "model/objectdetail.h"
#include "stringutil.h"


// convert object to QString
inline QString&
operator>>(msgpack::object obj, QString& str)
{
    if (obj.type != msgpack::type::RAW) { throw msgpack::type_error(); }

    msgpack::object_raw raw = obj.via.raw;
    str = QString::fromUtf8(raw.ptr, raw.size);
    return str;
}

// convert a map to key value.
template <typename K, typename V>
inline QMap<K, V>&
operator>>(msgpack::object obj, QMap<K, V>& map)
{
    if (obj.type != msgpack::type::MAP) { throw msgpack::type_error(); }

    msgpack::object_kv* list = obj.via.map.ptr;
    for (uint32_t iter = 0; iter != obj.via.map.size; ++iter) {
        auto key = list->key.as<K>();
        auto val = list->val.as<V>();

        map[key] = val;

        ++list;
    }

    return map;
}


// convert a list to QList
template <typename T>
inline QList<T>&
operator>>(msgpack::object obj, QList<T>& list)
{
    if (obj.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }

    msgpack::object_array l = obj.via.array;
    for (uint32_t iter = 0; iter != l.size; ++iter) {
        list.append(l.ptr[iter].as<T>());
    }

    return list;
}


// convert any value to QVariant.
inline QVariant&
operator>>(msgpack::object obj, QVariant& var)
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
inline RBKit::ObjectDetail&
operator>>(msgpack::object obj, RBKit::ObjectDetail& object)
{
    if (obj.type != msgpack::type::MAP) { throw msgpack::type_error(); }

    auto map = obj.as< QMap<int, msgpack::object> >();

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


// convert a payload into objectdetail pointer. not sure, whether we should
// have this or not.
inline RBKit::ObjectDetailPtr&
operator>>(msgpack::object obj, RBKit::ObjectDetailPtr& objectPtr)
{
    if (obj.type != msgpack::type::MAP) { throw msgpack::type_error(); }

    objectPtr.reset(new RBKit::ObjectDetail());
    obj >> *objectPtr;
    return objectPtr;
}

#endif // RBKIT_MSGPACK_PARSER_H
