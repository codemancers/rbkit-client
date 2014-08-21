#ifndef TESTHEAPTABLE_H
#define TESTHEAPTABLE_H

#include "AutoTest.h"
#include "heaptable.h"
#include "objectstore.h"

class TestHeapTable : public QObject
{
    Q_OBJECT
public:
    TestHeapTable();
public slots:
    void initTestCase();
    void cleanupTestCase();
};

#endif // TESTHEAPTABLE_H
