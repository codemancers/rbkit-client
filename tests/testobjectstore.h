#ifndef TESTOBJECTSTORE_H
#define TESTOBJECTSTORE_H

#include "AutoTest.h"
#include "objectstore.h"

class TestObjectStore : public QObject
{
    Q_OBJECT
public:
    TestObjectStore();
private slots:
    void testGetObject();
};
DECLARE_TEST(TestObjectStore)
#endif // TESTOBJECTSTORE_H
