#ifndef TEST_RBKIT_JSBRIDGE_H
#define TEST_RBKIT_JSBRIDGE_H

#include "AutoTest.h"
#include "model/jsbridge.h"

class TestJsBridge : public QObject
{
    Q_OBJECT

private slots:
    void receiveData(const QVariantMap& data);
    void cleanup();
    void init();

    // tests which need to be performed
    void testSendMapToJs();

private:
    RBKit::JsBridge* jsBridge;
    QVariantMap receivedData;
};

#endif // TEST_RBKIT_JSBRIDGE_H
