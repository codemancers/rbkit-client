#include "jsbridge.h"


void RBKit::JsBridge::sendObjectStats(const QVariantMap& map)
{
    QVariantMap hash;
    hash["event_type"] = QString("object_stats");
    hash["payload"] = map;

    emit jsEvent(hash);
}

void RBKit::JsBridge::sendGcStats(const QVariantMap& map)
{
    QVariantMap hash;
    hash["event_type"] = QString("gc_stats");
    hash["payload"] = map;

    emit jsEvent(hash);
}
