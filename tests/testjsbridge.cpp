#include "testjsbridge.h"
#include <QDateTime>
#include <QDebug>


using namespace RBKit;


// test whether object stats are modified and sent to js bridge.
void TestJsBridge::testSendMapToJs()
{
    QDateTime current = QDateTime::currentDateTime();

    QVariantMap testStats;
    testStats["String"] = 100;

    jsBridge->sendMapToJs("object_stats", current, testStats);

    QVERIFY("object_stats" == receivedData["event_type"]);
    QVariantMap payload = receivedData["payload"].toMap();
    QVERIFY(100 == payload["String"]);
}


void TestJsBridge::receiveData(const QVariantMap& data)
{
    receivedData = data;
}

void TestJsBridge::init()
{
    jsBridge = new JsBridge();
    connect(jsBridge, SIGNAL(jsEvent(const QVariantMap&)),
            this, SLOT(receiveData(const QVariantMap&)));

    receivedData.clear();
}

void TestJsBridge::cleanup()
{
    delete jsBridge;
}
