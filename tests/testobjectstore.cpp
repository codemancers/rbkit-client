#include "testobjectstore.h"

TestObjectStore::TestObjectStore()
{
}

void TestObjectStore::testGetObject()
{
    ObjectStore *objectStore = new ObjectStore();
    ObjectDetail *objectDetail = new ObjectDetail("String", 12345);

    objectStore->addObject(objectDetail);

    ObjectDetail *foundObject = objectStore->getObject(12345);
    QVERIFY(foundObject);

    QCOMPARE(objectDetail->className, foundObject->className);

    ObjectDetail *nullObject = objectStore->getObject(1234);
    QVERIFY2(nullObject == NULL, "result value was not null");
}
