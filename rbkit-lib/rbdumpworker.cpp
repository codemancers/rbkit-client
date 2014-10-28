#include "sqlconnectionpool.h"
#include "rbdumpworker.h"
#include "mpparser.h"
#include "debug.h"



RBKit::RbDumpParser::RbDumpParser(msgpack::unpacked dump)
    : heapDump(dump)
{
    auto map = dump.get().as< QMap<QString, msgpack::object> >();
    objectArray = map["payload"];
}


void RBKit::RbDumpWorker::dump(msgpack::unpacked dump)
{
    ENTER0("");

    auto connection = RBKit::SqlConnectionPool::getInstance();
    RBKit::RbDumpParser parser(dump);

    connection->persistObjects(parser);
}
