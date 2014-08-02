#ifndef RBKIT_EVENTS_H
#define RBKIT_EVENTS_H

#include <QVariantMap>
#include <QDateTime>
#include <QString>

class Subscriber;               // this acts as processor also atm.

namespace RBKit
{
    class EventDataBase
    {
    public:
        EventDataBase(QDateTime ts);
        virtual void process(Subscriber& processor) const = 0;

        QDateTime timestamp;
    };

    class EvtNewObject : public EventDataBase
    {
    public:
        EvtNewObject(QDateTime ts, QVariantMap payload);
        void process(Subscriber& processor) const;

        QString className;
        quint64 objectId;
    };

    class EvtDelObject : public EventDataBase
    {
    public:
        EvtDelObject(QDateTime ts, QVariantMap payload);
        void process(Subscriber& processor) const;

        quint64 objectId;
    };

    const EventDataBase* parseEvent(const QByteArray& rawMessage);
}


#endif // RBKIT_EVENTS_H
