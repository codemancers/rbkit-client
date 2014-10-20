#include "sqlconnectionpool.h"
#include "rbheapworker.h"
#include "rbevents.h"
#include "rbdebug.h"



RBKit::RbDumpParser::RbDumpParser(const QByteArray message)
    : rawMessage(message)
{
}


void RBKit::RbDumpParser::parse()
{
    eventParser.reset(new RBKit::EventParser(rawMessage));
    heapDump = eventParser->extractObjectDump();
    INFO1("heap type: %d", heapDump.type);
}


void RBKit::RbHeapWorker::dump(const QByteArray rawMessage)
{
    ENTER0("");

    auto connection = RBKit::SqlConnectionPool::getInstance();
    RBKit::RbDumpParser parser(rawMessage);
    parser.parse();

    connection->persistObjects(parser);

    emit dumpAvailable(connection->getCurrentVersion());
}
