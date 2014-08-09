#include "testjsbridge.h"
#include <QDebug>

using namespace RBKit;


// test whether object stats are modified and sent to js bridge.
void TestJsBridge::testSendObjectStats()
{
    QVariantMap testStats;
    testStats["String"] = 100;

    jsBridge->sendObjectStats(testStats);

    QVERIFY("object_stats" == receivedData["event_type"]);
    QVariantMap payload = receivedData["payload"].toMap();
    QVERIFY(100 == payload["String"]);
}


// test whether gc-stats are modified and sent to js bridge.
void TestJsBridge::testSendGcStats()
{
    QVariantMap gcStats;
    gcStats["count"] = 46;

    jsBridge->sendGcStats(gcStats);

    QVERIFY("gc_stats" == receivedData["event_type"]);
    QVariantMap payload = receivedData["payload"].toMap();
    QVERIFY(46 == payload["count"]);
}


void TestJsBridge::receiveData(const QVariantMap& data)
{
    receivedData = data;
}

void TestJsBridge::init()
{
    jsBridge = new JsBridge(this);
    connect(jsBridge, SIGNAL(jsEvent(const QVariantMap&)),
            this, SLOT(receiveData(const QVariantMap&)));

    receivedData.clear();
}

void TestJsBridge::cleanup()
{
    delete jsBridge;
}
