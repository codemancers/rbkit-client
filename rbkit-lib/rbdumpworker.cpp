#include "sqlconnectionpool.h"
#include "rbdumpworker.h"
#include "rbevents.h"
#include "debug.h"



RBKit::RbDumpParser::RbDumpParser(msgpack::object dump)
    : heapDump(dump)
{
}


void RBKit::RbDumpWorker::dump(const QByteArray rawMessage)
{
    ENTER0("");

    RBKit::EventParser rawParser(rawMessage);
    auto dump = rawParser.extractObjectDump();
    INFO1("dump type: %d", dump.type);

    auto connection = RBKit::SqlConnectionPool::getInstance();
    RBKit::RbDumpParser parser(rawParser.extractObjectDump());

    connection->persistObjects(parser);

    emit dumpAvailable(connection->getCurrentVersion());
}
