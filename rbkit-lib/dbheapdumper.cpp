#include "model/objectdetail.h"
#include "dbheapdumper.h"
#include "mpparser.h"
#include <QDebug>


void RBKit::DbHeapDumper::dump(msgpack::unpacked heapDump)
{
    auto map = heapDump.get().as< QMap<QString, msgpack::object> >();
    auto payload = map["payload"];


    msgpack::object_array list = obj.via.array;

    for (uint32_t iter = 0; iter != list.size; ++iter) {
        RBKit::ObjectDetail object;
        list.ptr[iter] >> object;

        persistObjectToDb(object);
    }
}


void RBKit::DbHeapDumper::persistObjectToDb(RBKit::ObjectDetail& object)
{
    
}
