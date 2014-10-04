#ifndef TESTSNAPSHOTSTATE_H
#define TESTSNAPSHOTSTATE_H

#include "AutoTest.h"
#include "model/snapshotstate.h"
#include "ui/heapdumpform.h"

using namespace RBKit;

class TestSnapshotState : public QObject
{
    Q_OBJECT
private:
    SnapshotState *snapshotState;
private slots:
    void testAddTab();
    void testRemoveTab();
    void testAddManyRemoveManyTabs();
    void init();
    void cleanup();
};

DECLARE_TEST(TestSnapshotState)

#endif // TESTSNAPSHOTSTATE_H
