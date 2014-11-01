#include "testobjectdump.h"
#include "rbevents.h"
#include "mpparser.h"
#include "model/objectstore.h"
#include "rbheapworker.h"
#include <QDebug>

using namespace RBKit;

static QByteArray msgpackDataFromFile(const QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    return file.readAll();
}

void TestObjectDump::initTestCase()
{
    // read object dump, and parse it
    objectDump = msgpackDataFromFile(":/tests/msgpack/hugedump");

    EventParser eventParser(objectDump);
    EvtCollection* coll = dynamic_cast<EvtCollection*>(eventParser.parseEvent());
    event.reset( dynamic_cast<EvtObjectDump*>(coll->events[0].data()) );
}

void TestObjectDump::testBenchmarkParseObjectDump()
{
    EventDataBase* base = NULL;

    QBENCHMARK {
        EventParser eventParser(objectDump);
        base = eventParser.parseEvent();
    }

    QVERIFY(base);
}


void TestObjectDump::testBenchmarkExtractObjectDump()
{
    EventParser eventParser(objectDump);
    msgpack::object dump;

    QBENCHMARK_ONCE {
        dump = eventParser.extractObjectDump();
    }

    QVERIFY(msgpack::type::ARRAY == dump.type);
}


void TestObjectDump::testBenchmarkHeapDumpParserParse()
{
    QBENCHMARK_ONCE {
        RbDumpParser parser(objectDump);
        parser.parse();
    }
}


void TestObjectDump::testBenchmarkIterateParsedHeapDump()
{
    RbDumpParser parser(objectDump);
    parser.parse();

    int objects(0);

    QBENCHMARK_ONCE {
        auto iter = parser.begin();
        for (; iter != parser.end(); ++iter) {
            ++objects;
        }
    }

    QVERIFY(11326 == objects);
}


void TestObjectDump::testBenchmarkParseAndConvertToObjects()
{
    RbDumpParser parser(objectDump);
    parser.parse();

    int objects(0);

    QBENCHMARK_ONCE {
        auto iter = parser.begin();
        for (; iter != parser.end(); ++iter) {
            ObjectDetail object;
            *iter >> object;

            if (object.objectId > 0) {
                ++objects;
            }
        }
    }

    QVERIFY(11326 == objects);
}
