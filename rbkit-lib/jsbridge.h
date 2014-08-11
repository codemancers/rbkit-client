#ifndef RBKIT_JSEVENT_BRIDGE_H
#define RBKIT_JSEVENT_BRIDGE_H

#include <QObject>
#include <QVariantMap>
#include <QDateTime>
#include <QString>


namespace RBKit
{
    class JsBridge : public QObject
    {
        Q_OBJECT

    public:
        ~JsBridge() {}

    public:
        void sendMapToJs(const QString& event, const QDateTime& ts,
                         const QVariantMap& map);

    signals:
        void jsEvent(const QVariantMap& map);
    };
}


#endif // RBKIT_JSEVENT_BRIDGE_H
