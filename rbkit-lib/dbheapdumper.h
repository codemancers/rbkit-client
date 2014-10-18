#ifndef RBKIT_HEAP_DUMPER_H
#define RBKIT_HEAP_DUMPER_H

#include <QSharedPointer>
#include <msgpack.hpp>


namespace RBKit
{
    class ObjectDetail;

    class DbHeapDumper : public QObject
    {
        Q_OBJECT;

        msgpack::unpacked heapDump;

    public:
        void persistObjectToDb(const ObjectDetail&);

    public slots:
        void dump(msgpack::unpacked dump);

    };
}

#endif // RBKIT_HEAP_DUMPER_H
