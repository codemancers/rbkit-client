#ifndef TESTAPPSTATE_H
#define TESTAPPSTATE_H

#include "AutoTest.h"
#include "model/appstate.h"

class TestAppState : public QObject
{
    Q_OBJECT
public:
    TestAppState();
public slots:
    void testAppState();
};

#endif // TESTAPPSTATE_H
