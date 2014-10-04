#include "testsnapshotstate.h"


void TestSnapshotState::testAddTab()
{
    HeapDumpFormPtr heapForm = HeapDumpFormPtr(new HeapDumpForm(0, 0));
    snapshotState->addNewSnapshot(heapForm, "snapshot #1");

    QVERIFY(1 == snapshotState->getSnapShotIndex());
}

void TestSnapshotState::testRemoveTab()
{

}

void TestSnapshotState::testAddManyRemoveManyTabs()
{

}

void TestSnapshotState::init()
{
    snapshotState = new SnapshotState();
}

void TestSnapshotState::cleanup()
{
   snapshotState->reset();
}


