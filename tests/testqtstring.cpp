#include "testqtstring.h"

TestQtString::TestQtString()
{
}

void TestQtString::testQtString()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}
