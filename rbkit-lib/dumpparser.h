#ifndef RBKIT_DUMP_PARSER_H
#define RBKIT_DUMP_PARSER_H

#include <QSharedPointer>
#include <msgpack.hpp>


namespace RBKit
{
    class ObjectDetail;

    class DumpParser : public QObject
    {
        Q_OBJECT;

        msgpack::unpacked heapDump;

    public slots:
        void dump(msgpack::unpacked dump);

    public:                     // add iterator interface.
        class Iterator
        {
        public:
            Iterator() : ptr(NULL) {}
            Iterator(msgpack::object* ptr_) : ptr(ptr_) {}

        public:
            Iterator operator++(int) { return Iterator(ptr++); }
            Iterator operator++() { return Iterator(++ptr); }
            msgpack::object& operator*() { return *ptr; }

        private:
            msgpack::object* ptr;
        };

    public:                     // add begin, end interface.
        Iterator begin() {
            auto ptr = heapDump.get().via.array.ptr;
            return Iterator(ptr);
        }

        Iterator end() {
            auto array = heapDump.get().via.array;
            return Iterator(array.ptr + array.size);
        }
    };
}

#endif // RBKIT_HEAP_DUMPER_H
