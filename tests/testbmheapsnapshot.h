#ifndef TEST_RBKIT_BENCHMARK_HEAPSNAPSHOT_H
#define TEST_RBKIT_BENCHMARK_HEAPSNAPSHOT_H

#include <QSharedPointer>
#include "AutoTest.h"

// forward declaration
namespace RBKit
{
    class ObjectStore;
}

class TestBmHeapSnapshot : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testBenchmarkPersistingToDb();

private:
    QSharedPointer<RBKit::ObjectStore> store;
};

DECLARE_TEST(TestBmHeapSnapshot)

#endif // TEST_RBKIT_BENCHMARK_HEAPSNAPSHOT_H
