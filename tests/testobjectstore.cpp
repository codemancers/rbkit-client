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

void TestObjectStore::testRemoveObject()
{
    this->objectStore->reset();
    RBKit::ObjectDetailPtr objectDetail(new RBKit::ObjectDetail("String", 12345));
    objectStore->addObject(objectDetail);

    QVERIFY2(objectStore->getObjectTypeCount("String") == 1, "does not have correct String count");

    objectStore->removeObject(12345);
    QVERIFY2(objectStore->getObjectTypeCount("String") == 0, "does not have correct String count");
}

void TestObjectStore::testObjectTypeMap()
{
    this->objectStore->reset();

    RBKit::ObjectDetailPtr objectDetail(new RBKit::ObjectDetail("String", 12345));
    objectStore->addObject(objectDetail);

    QVariantMap map = objectStore->getObjectTypeCountMap();
    QVERIFY(map.size() == 1);
    quint32 count = map["String"].toInt();
    QVERIFY(count == 1);
}

void TestObjectStore::testUpdateGeneration()
{
    this->objectStore->reset();

    RBKit::ObjectDetailPtr objectDetail(new RBKit::ObjectDetail("String", 12345));
    objectStore->addObject(objectDetail);
    QVERIFY(objectStore->objectStore[12345]->objectGeneration == 0);

    objectStore->updateObjectGeneration();
    QVERIFY(objectStore->objectStore[12345]->objectGeneration == 1);
}

void TestObjectStore::testObjectDetailGetFileLine()
{
    RBKit::ObjectDetail *objectDetail = new RBKit::ObjectDetail("String", 12345);
    objectDetail->fileName = "foo.rb";
    objectDetail->lineNumber = 10;

    QString fileLine = objectDetail->getFileLine();
    QVERIFY(fileLine == "foo.rb:10");
}
