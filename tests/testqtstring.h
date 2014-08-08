#ifndef TESTQTSTRING_H
#define TESTQTSTRING_H
#include "AutoTest.h"

class TestQtString : public QObject
{
    Q_OBJECT
public:
    TestQtString();
private slots:
    void testQtString();
};

DECLARE_TEST(TestQtString)

#endif // TESTQTSTRING_H
