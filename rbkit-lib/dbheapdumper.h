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
        void DbHeapDumper();

    public slots:
        void dump(msgpack::unpacked dump);

    private:
        void persistObjectToDb(const ObjectDetail&);
        void prepareDatabase();
        void beginBatchInsert();
        void finalizeBatchInsert();
    };
}

#endif // RBKIT_HEAP_DUMPER_H
