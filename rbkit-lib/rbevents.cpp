#include <msgpack.hpp>
#include "subscriber.h"
#include "rbevents.h"

#include <QDebug>


static inline QString rawToQString(msgpack::object obj)
{
    msgpack::object_raw raw = obj.via.raw;
    return QString::fromUtf8(raw.ptr, raw.size);
}


// NOTE: This can be improved with the version that hemant is writing for GCStats.
static QVariantMap parseMsgpackObjectMap(msgpack::object_map obj)
{
    QVariantMap map;

    msgpack::object_kv* list = obj.ptr;
    for (uint32_t iter = 0; iter != obj.size; ++iter) {
        msgpack::object key = list->key;
        msgpack::object val = list->val;

        // qDebug() << key.type << val.type;

        QString keyStr = rawToQString(key);
        switch (val.type) {
        case msgpack::type::MAP : // recursion here.
            map[keyStr] = parseMsgpackObjectMap(val.via.map);
            break;

        case msgpack::type::RAW :
            map[keyStr] = rawToQString(val);
            break;
        case msgpack::type::DOUBLE :
            map[keyStr] = val.via.dec;
            break;
        case msgpack::type::POSITIVE_INTEGER :
            map[keyStr] = val.via.u64;
            break;
        default:
            throw "unknown object type";
        }

        ++list;
    }

    return map;
}

const RBKit::EventDataBase* RBKit::parseEvent(const QByteArray& message)
{
    msgpack::unpacked unpackedMessage;
    msgpack::unpack(&unpackedMessage, message.data(), message.size());

    msgpack::object_map obj = unpackedMessage.get().via.map;

    QVariantMap map = parseMsgpackObjectMap(obj);
    // qDebug() << map << map["payload"];

    QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(map["timestamp"].toULongLong());
    if (map["event_type"] == "obj_created") {
        return new RBKit::EvtNewObject(timestamp, map["payload"].toMap());
    } else if (map["event_type"] == "obj_destroyed") {
        return new RBKit::EvtNewObject(timestamp, map["payload"].toMap());
    } else {
        return NULL;
    }
}



RBKit::EventDataBase::EventDataBase(QDateTime ts)
    : timestamp(ts)
{
}


RBKit::EvtNewObject::EvtNewObject(QDateTime ts, QVariantMap payload)
    : EventDataBase(ts)
    , objectId(payload["object_id"].toULongLong())
    , className(payload["class"].toString())
{
}

void RBKit::EvtNewObject::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}

RBKit::EvtDelObject::EvtDelObject(QDateTime ts, QVariantMap payload)
    : EventDataBase(ts)
    , objectId(payload["object_id"].toULongLong())
{ }

void RBKit::EvtDelObject::process(Subscriber& processor) const
{
    processor.processEvent(*this);
}
