#ifndef RBKIT_MSGPACK_PARSER_H
#define RBKIT_MSGPACK_PARSER_H


#include "model/objectdetail.h"
#include <msgpack.hpp>



// convert a map to key value.
template <typename K, typename V>
inline QMap<K, V>& operator>>(msgpack::object obj, QMap<K, V>& map)
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
inline QList<T>& operator>>(msgpack::object obj, QList<T>& list)
{
    if (obj.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }

    msgpack::object_array l = obj.via.array;
    for (uint32_t iter = 0; iter != l.size; ++iter) {
        list.append(l.ptr[iter].as<T>());
    }

    return list;
}


// convert object to QString
QString& operator>>(msgpack::object obj, QString& string);


// convert any value to QVariant.
QVariant& operator>>(msgpack::object obj, QVariant& var);


// convert a payload into objectdetail pointer. not sure, whether we should
// have this or not.
RBKit::ObjectDetailPtr&
operator>>(msgpack::object obj, RBKit::ObjectDetailPtr& objectPtr);
RBKit::ObjectDetail&
operator>>(msgpack::object obj, RBKit::ObjectDetail& object);


#endif // RBKIT_MSGPACK_PARSER_H
