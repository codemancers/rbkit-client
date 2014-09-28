#include "snapshotstate.h"

SnapshotState::SnapshotState()
    : snapShotIndex(0)
{
}

void SnapshotState::addNewSnapshot(HeapDumpForm *form)
{
    ++snapShotIndex;
    heapForms[snapShotIndex] = form;
}

void SnapshotState::removeSnapshot(int index)
{

}

bool SnapshotState::snapShotInProgress()
{

}

int SnapshotState::getSnapshotProgress()
{

}

void SnapshotState::setSnapshotProgress()
{

}

QList<int> SnapshotState::diffableSnapshotVersions() {
    QList<int> selectedSnapshots;
    QMapIterator<int, HeapDumpForm *> iterator(heapForms);
    while (iterator.hasNext()) {
        iterator.next();
        HeapDumpForm *form = iterator.value();
        if (form->getRootItem()->getIsSnapshot()) {
            selectedSnapshots.append(iterator.key());
        }
    }
    return selectedSnapshots;
}
