#include "sqlconnectionpool.h"
#include "rbdumpworker.h"
#include "mpparser.h"
#include <QDebug>



RBKit::RbDumpParser::RbDumpParser(msgpack::unpacked dump)
    : heapDump(dump)
{
    auto map = dump.get().as< QMap<QString, msgpack::object> >();
    objectArray = map["payload"];
}


void RBKit::RbDumpWorker::dump(msgpack::unpacked dump)
{
    auto connection = RBKit::SqlConnectionPool::getInstance();
    RBKit::RbDumpParser parser(dump);

    connection->persistObjects(parser);
}
