#include "testobjectstore.h"

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

void TestObjectStore::testGetObject()
{
    RBKit::ObjectDetail *objectDetail = new RBKit::ObjectDetail("String", 12345);

    objectStore->addObject(objectDetail);

    RBKit::ObjectDetail *foundObject = objectStore->getObject(12345);
    QVERIFY(foundObject);
    QVERIFY2(objectStore->getObjectTypeCount("String") == 1, "does not have correct String count");

    QCOMPARE(objectDetail->className, foundObject->className);

    RBKit::ObjectDetail *nullObject = objectStore->getObject(1234);
    QVERIFY2(nullObject == NULL, "result value was not null");
}

void TestObjectStore::testRemoveObject()
{
    this->objectStore->reset();
    RBKit::ObjectDetail *objectDetail = new RBKit::ObjectDetail("String", 12345);
    objectStore->addObject(objectDetail);

    QVERIFY2(objectStore->getObject(12345) != NULL, "fetching valid object is NULL");
    QVERIFY2(objectStore->getObjectTypeCount("String") == 1, "does not have correct String count");

    objectStore->removeObject(12345);
    QVERIFY2(objectStore->getObject(12345) == NULL, "fetching invalid is not NULL");
    QVERIFY2(objectStore->getObjectTypeCount("String") == 0, "does not have correct String count");
}

void TestObjectStore::testObjectTypeMap()
{
    this->objectStore->reset();

    RBKit::ObjectDetail *objectDetail = new RBKit::ObjectDetail("String", 12345);
    objectStore->addObject(objectDetail);

    QVariantMap map = objectStore->getObjectTypeCountMap();
    QVERIFY(map.size() == 1);
    quint32 count = map["String"].toInt();
    QVERIFY(count == 1);
}

void TestObjectStore::testUpdateGeneration()
{
    this->objectStore->reset();

    RBKit::ObjectDetail *objectDetail = new RBKit::ObjectDetail("String", 12345);
    objectStore->addObject(objectDetail);
    QVERIFY(objectStore->objectStore[12345]->objectGeneration == 0);

    objectStore->updateObjectGeneration();
    QVERIFY(objectStore->objectStore[12345]->objectGeneration == 1);
}
