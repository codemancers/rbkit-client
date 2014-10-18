#include "model/objectdetail.h"
#include "dbheapdumper.h"
#include "mpparser.h"
#include <QDebug>


static const unsigned int batchSize = 1000;


void RBKit::DbHeapDumper::dump(msgpack::unpacked heapDump)
{
    auto map = heapDump.get().as< QMap<QString, msgpack::object> >();
    auto payload = map["payload"];


    msgpack::object_array list = obj.via.array;

    prepareDatabase();

    beginBatchInsert();
    for (uint32_t iter = 0; iter != list.size; ++iter) {
        RBKit::ObjectDetail object;
        list.ptr[iter] >> object;

        persistObjectToDb(object);

        if (0 == (iter % batchSize)) {
            finalizeBatchInsert();
            beginBatchInsert();
        }
    }
    finalizeBatchInsert();

    // emit task completion
}


void RBKit::DbHeapDumper::persistObjectToDb(RBKit::ObjectDetail& object)
{
    RBKit::SqlConnectionPool::getInstance()->persistObject(object);
}


void RBKit::DbHeapDumper::prepareDatabase()
{
    RBKit::SqlConnectionPool::getInstance()->prepareTables();
}


void RBKit::DbHeapDumper::beginBatchInsert()
{
    RBKit::SqlConnectionPool::getInstance()->beginTransaction();
}


void RBKit::DbHeapDumper::finalizeBatchInsert()
{
    RBKit::SqlConnectionPool::getInstance()->commitTransaction();
}
