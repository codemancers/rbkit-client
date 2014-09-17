#ifndef TEST_RBKIT_OBJECTDUMP_H
#define TEST_RBKIT_OBJECTDUMP_H

#include "AutoTest.h"

class TestObjectDump : public QObject
{
    Q_OBJECT

private slots:
    void testBenchmarkParseObjectDump();
    void testBenchmarkProcessObjectsWhenObjectSpaceIsEmpty();
    void testBenchmarkProcessObjectsWhenObjectSpaceIsFull();
};

DECLARE_TEST(TestObjectDump)

#endif // TEST_RBKIT_OBJECTDUMP_H
