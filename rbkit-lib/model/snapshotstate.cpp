#include "snapshotstate.h"
#include "appstate.h"

namespace RBKit {

SnapshotState::SnapshotState()
    : snapShotIndex(0), snapShotInProgressFlag(false)
{
}

void SnapshotState::addNewSnapshot(HeapDumpFormPtr form, QString snapshotName)
{
    ++snapShotIndex;
    heapForms[snapShotIndex] = form;
    RBKit::AppState::getInstance()->setSnapshotName(snapShotIndex, snapshotName);
}

void SnapshotState::removeSnapshot(int index)
{
    if (index == 0)
        return;
    if (heapForms.remove(index) > 0) {
        --snapShotIndex;
        RBKit::AppState::getInstance()->removeSnapshotName(index);
    }
}

bool SnapshotState::snapShotInProgress() const
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

HeapItem *SnapshotState::diffRootItem(QList<int> selectedSnapshots)
{
    HeapItem *item1 = heapForms[selectedSnapshots.at(0)]->getRootItem();
    HeapItem *item2 = heapForms[selectedSnapshots.at(1)]->getRootItem();

    return item2->minus(item1);
}

}
