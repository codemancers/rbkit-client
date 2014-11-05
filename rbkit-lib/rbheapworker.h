#ifndef RBKIT_HEAP_PARSER_H
#define RBKIT_HEAP_PARSER_H

#include <QSharedPointer>
#include <msgpack.hpp>
#include "model/objectdetail.h"


namespace RBKit
{
    class ObjectDetail;
    class RbDumpParser;
    class EventParser;

    class RbHeapWorker : public QObject
    {
        Q_OBJECT

    public slots:
        void dump(const QByteArray);

    signals:
        void parsedObjects(const RBKit::QHashObjectIdToPtr);
        void dumpAvailable(int);
    };


    class RbDumpParser : public QObject
    {
        Q_OBJECT

        const QByteArray rawMessage;

        QSharedPointer<EventParser> eventParser;
        msgpack::object heapDump;

    public:
        RbDumpParser(const QByteArray);
        void parse();

    public:                     // add iterator interface.
        typedef msgpack::object* Iterator;

        inline Iterator begin() {
            return heapDump.via.array.ptr;
        }

        inline Iterator end() {
            auto array = heapDump.via.array;
            return array.ptr + array.size;
        }
    };
}


#endif // RBKIT_HEAP_DUMPER_H
