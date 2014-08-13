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
        EventDataBase(QDateTime ts, QString eventName);
        virtual void process(Subscriber& processor) const = 0;

        QDateTime timestamp;
        QString eventName;
    };

    class EvtNewObject : public EventDataBase
    {
    public:
        EvtNewObject(QDateTime ts, QString eventName, QVariantMap payload);
        void process(Subscriber& processor) const;

        QString className;
        quint64 objectId;
    };

    class EvtDelObject : public EventDataBase
    {
    public:
        EvtDelObject(QDateTime ts, QString eventName, QVariantMap payload);
        void process(Subscriber& processor) const;

        quint64 objectId;
    };

    class EvtGcStats : public EventDataBase
    {
    public:
        EvtGcStats(QDateTime ts, QString eventName, QVariantMap payload);
        void process(Subscriber& processor) const;

        QVariantMap payload;
    };

    class EvtGcStart : public EventDataBase
    {
    public:
        EvtGcStart(QDateTime ts, QString eventName);
        void process(Subscriber &processor) const;
    };

    class EvtGcStop : public EventDataBase
    {
    public:
        EvtGcStop(QDateTime ts, QString eventName);
        void process(Subscriber &processor) const;
    };

    EventDataBase* parseEvent(const QByteArray& rawMessage);
}


#endif // RBKIT_EVENTS_H
