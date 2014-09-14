#ifndef TESTOBJECTSTORE_H
#define TESTOBJECTSTORE_H

#include "AutoTest.h"
#include "objectstore.h"

class TestObjectStore : public QObject
{
    Q_OBJECT
public:
    TestObjectStore();
    RBKit::ObjectStore *objectStore;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testRemoveObject();
    void testUpdateGeneration();
    void testObjectDetailGetFileLine();
};

DECLARE_TEST(TestObjectStore)
#endif // TESTOBJECTSTORE_H
