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
    objectDump = msgpackDataFromFile("/Users/yuva/00013-04-object-refs-to-ints");

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

    QVERIFY(1178271 == objects);
}


void TestObjectDump::testBenchmarkParseAndConvertToObjects()
{
    RbDumpParser parser(objectDump);
    parser.parse();

    quint64 objects(0);
    quint64 refs(0);

    QBENCHMARK_ONCE {
        auto iter = parser.begin();
        for (; iter != parser.end(); ++iter) {
            ObjectDetail object;
            *iter >> object;

            if (object.objectId > 0) {
                refs += object.references.size();
                ++objects;
            }
        }
    }

    QVERIFY(1178271 == objects);
    QVERIFY(2885823 == refs);
}
