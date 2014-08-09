#ifndef RBKIT_JSEVENT_BRIDGE_H
#define RBKIT_JSEVENT_BRIDGE_H

#include <QObject>
#include <QVariantMap>


namespace RBKit
{
    class JsBridge : public QObject
    {
        Q_OBJECT

    public:
        JsBridge(QObject* parent) : QObject(parent) {}
        ~JsBridge() {}

    public:
        void sendObjectStats(const QVariantMap& map);
        void sendGcStats(const QVariantMap& map);

    signals:
        void jsEvent(const QVariantMap& map);
    };
}


#endif // RBKIT_JSEVENT_BRIDGE_H
