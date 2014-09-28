#include "snapshotstate.h"

namespace RBKit {

SnapshotState::SnapshotState()
    : snapShotIndex(0)
{
}

void SnapshotState::addNewSnapshot(HeapDumpFormPtr form)
{
    ++snapShotIndex;
    heapForms[snapShotIndex] = form;
}

void SnapshotState::removeSnapshot(int index)
{
    if (index == 0)
        return;
    if (heapForms.remove(index) > 0) {
        --snapShotIndex;
    }
}

bool SnapshotState::snapShotInProgress()
{
    return snapShotInProgressFlag;
}

void SnapshotState::setSnapshotProgress(bool snapShotState)
{
    snapShotInProgressFlag = snapShotState;
}

QList<int> SnapshotState::diffableSnapshotVersions() {
    QList<int> selectedSnapshots;
    QMapIterator<int, HeapDumpFormPtr> iterator(heapForms);
    while (iterator.hasNext()) {
        iterator.next();
        HeapDumpFormPtr form = iterator.value();
        if (form->getRootItem()->getIsSnapshot()) {
            selectedSnapshots.append(iterator.key());
        }
    }
    return selectedSnapshots;
}

}
