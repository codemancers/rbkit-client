#ifndef TESTAPPSTATE_H
#define TESTAPPSTATE_H

#include "AutoTest.h"
#include "appstate.h"

class TestAppState : public QObject
{
    Q_OBJECT
public:
    TestAppState();
public slots:
    void testAppState();
};

DECLARE_TEST(TestAppState)

#endif // TESTAPPSTATE_H
