#ifndef RBKIT_EVENT_PARSER_H
#define RBKIT_EVENT_PARSER_H

#include "rbevents.h"

namespace RBKit
{
    // https://github.com/code-mancers/rbkit/blob/47b461/ext/rbkit_event_packer.h#L6
    enum EventField
    {
        EfEventType       = 0,
        EfTimestamp       = 1,
        EfPayload         = 2,
        EfMessageCounter  = 9
    };


    class EventParser
    {
    public:
        EventParser(const QByteArray& message);

    public:
        EventDataBase* parseEvent() const;
        EvtHandshake* parseHandShake() const;

    public:                     // helpers
        RBKit::EventDataBase* eventFromMsgpackObject(const msgpack::object&) const;
        QList<RBKit::EventPtr> parseEvtArray(const msgpack::object&) const;
        RBKit::EventType guessEvent(const msgpack::object&) const;

    public:
        QList<RBKit::ObjectDetailPtr> parseObjects(const msgpack::object&) const;

    private:
        const QByteArray rawMessage;
        msgpack::unpacked unpacked;
    };
}


#endif // RBKIT_EVENT_PARSER_H
