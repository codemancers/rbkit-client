#ifndef RBKIT_DUMP_PARSER_H
#define RBKIT_DUMP_PARSER_H

#include <QSharedPointer>
#include <msgpack.hpp>


namespace RBKit
{
    class ObjectDetail;
    class RbDumpParser;

    class RbDumpWorker : public QObject
    {
        Q_OBJECT;

    public slots:
        void dump(const QByteArray);

    signals:
        void dumpAvailable(int);
    };


    class RbDumpParser : public QObject
    {
        Q_OBJECT;

        msgpack::object heapDump;

    public:
        RbDumpParser(msgpack::object dump);

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
