#include "jsbridge.h"


void RBKit::JsBridge::sendMapToJs(const QString& event, const QDateTime& ts,
                                  const QVariantMap& map)
{
    QVariantMap hash;
    hash["event_type"] = event;
    hash["timestamp"] = ts;
    hash["payload"] = map;

    emit jsEvent(hash);
}
