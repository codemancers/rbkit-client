#include "testobjectstore.h"

DECLARE_TEST(TestObjectStore)


TestObjectStore::TestObjectStore()
{
    this->objectStore = new RBKit::ObjectStore();
}

void TestObjectStore::initTestCase()
{
}

void TestObjectStore::cleanupTestCase()
{
    this->objectStore->reset();
}

void TestObjectStore::testRemoveObject()
{
    this->objectStore->reset();
    RBKit::ObjectDetailPtr objectDetail(new RBKit::ObjectDetail("String", 12345));
    objectStore->addObject(objectDetail);

    QVERIFY2(objectStore->hasKey(12345), "does not have correct String count");

    objectStore->removeObject(12345);
    QVERIFY2(!objectStore->hasKey(12345), "does not have correct String count");
}

void TestObjectStore::testUpdateGeneration()
{
    this->objectStore->reset();

    RBKit::ObjectDetailPtr objectDetail(new RBKit::ObjectDetail("String", 12345));
    objectStore->addObject(objectDetail);
    QVERIFY(objectDetail->objectGeneration == 0);

    objectStore->updateObjectGeneration();
    QVERIFY(objectDetail->objectGeneration == 1);
}

void TestObjectStore::testObjectDetailGetFileLine()
{
    RBKit::ObjectDetailPtr objectDetail(new RBKit::ObjectDetail("String", 12345));
    objectDetail->fileName = "foo.rb";
    objectDetail->lineNumber = 10;

    QString fileLine = objectDetail->getFileLine();
    QVERIFY(fileLine == "foo.rb:10");
}
