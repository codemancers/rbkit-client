#include <QtTest/QtTest>
#include <QThread>

#include "objectstore.h"
#include "objectdetail.h"

class TestObjectStore : public QObject
{
    Q_OBJECT

private slots:
    void testAddObject();
};

void TestObjectStore::testAddObject() {
    ObjectStore *store = new ObjectStore();
    ObjectDetail *objectDetail = new ObjectDetail("String", 12345);

    store->addObject(objectDetail);

    ObjectDetail objectDetail = store->getObject(12345);
}
