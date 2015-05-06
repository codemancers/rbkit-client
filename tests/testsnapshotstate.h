#ifndef TESTSNAPSHOTSTATE_H
#define TESTSNAPSHOTSTATE_H

#include "AutoTest.h"
#include "model/snapshotstate.h"
#include "ui/heapdumpform.h"

class TestSnapshotState : public QObject
{
    Q_OBJECT
public:
    TestSnapshotState();
private:
    RBKit::SnapshotState *snapshotState;
private slots:
    void testAddTab();
    void testRemoveTab();
    void testAddManyRemoveManyTabs();
    void init();
    void cleanup();
};

#endif // TESTSNAPSHOTSTATE_H
