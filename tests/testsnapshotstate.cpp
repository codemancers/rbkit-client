#include "testsnapshotstate.h"


TestSnapshotState::TestSnapshotState()
{
   this->snapshotState = new RBKit::SnapshotState();
}

void TestSnapshotState::testAddTab()
{
    HeapDumpFormPtr heapForm = HeapDumpFormPtr(0);
    snapshotState->addNewSnapshot(heapForm, "snapshot #1");

    QVERIFY(1 == snapshotState->getSnapShotIndex());
    QVERIFY(2 == snapshotState->getTabIndexToSnapshot().size());
}

void TestSnapshotState::testRemoveTab()
{
    HeapDumpFormPtr heapForm = HeapDumpFormPtr(0);
    snapshotState->addNewSnapshot(heapForm, "snapshot #1");

    QVERIFY(1 == snapshotState->getSnapShotIndex());
    snapshotState->removeSnapshot(1);

    QVERIFY(1 == snapshotState->getSnapShotIndex());
    QVERIFY(1 == snapshotState->getTabIndexToSnapshot().size());
}

void TestSnapshotState::testAddManyRemoveManyTabs()
{
    HeapDumpFormPtr heapForm = HeapDumpFormPtr(0);
    snapshotState->addNewSnapshot(heapForm, "snapshot #1");

    heapForm = HeapDumpFormPtr(0);
    snapshotState->addNewSnapshot(heapForm, "snapshot #2");

    heapForm = HeapDumpFormPtr(0);
    snapshotState->addNewSnapshot(heapForm, "snapshot #3");

    heapForm = HeapDumpFormPtr(0);
    snapshotState->addNewSnapshot(heapForm, "snapshot #4");


    heapForm = HeapDumpFormPtr(0);
    snapshotState->addNewSnapshot(heapForm, "snapshot #5");

    QVERIFY(5 == snapshotState->getSnapShotIndex());
    QVERIFY(6 == snapshotState->getTabIndexToSnapshot().size());

    snapshotState->removeSnapshot(2);

    QVERIFY(5 == snapshotState->getSnapShotIndex());
    QVERIFY(5 == snapshotState->getTabIndexToSnapshot().size());

    int snapshotIndex = snapshotState->getTabIndexToSnapshot().at(2);
    QVERIFY(3 == snapshotIndex);

    snapshotState->removeSnapshot(3);
    QVERIFY(5 == snapshotState->getSnapShotIndex());
    QVERIFY(4 == snapshotState->getTabIndexToSnapshot().size());

    snapshotIndex = snapshotState->getTabIndexToSnapshot().at(3);
    QVERIFY(5 == snapshotIndex);
}

void TestSnapshotState::init()
{
}

void TestSnapshotState::cleanup()
{
   this->snapshotState->reset();
}


