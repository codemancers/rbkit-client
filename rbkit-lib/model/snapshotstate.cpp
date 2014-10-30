#include "snapshotstate.h"
#include "appstate.h"

namespace RBKit {
int SnapshotState::getSnapShotIndex() const
{
    return snapShotIndex;
}

void SnapshotState::setSnapShotIndex(int value)
{
    snapShotIndex = value;
}

QMap<int, HeapDumpFormPtr> SnapshotState::getHeapForms() const
{
    return heapForms;
}

void SnapshotState::setHeapForms(const QMap<int, HeapDumpFormPtr> &value)
{
    heapForms = value;
}

QVector<int> SnapshotState::getTabIndexToSnapshot() const
{
    return tabIndexToSnapshot;
}

void SnapshotState::setTabIndexToSnapshot(const QVector<int> &value)
{
    tabIndexToSnapshot = value;
}
SnapshotState::SnapshotState()
    : snapShotIndex(0), snapShotInProgressFlag(false)
{
    tabIndexToSnapshot.append(0);
}

void SnapshotState::addNewSnapshot(HeapDumpFormPtr form, QString snapshotName)
{
    ++snapShotIndex;
    heapForms[snapShotIndex] = form;
    tabIndexToSnapshot.append(snapShotIndex);
    qDebug() << "+ Adding new snapshot " << snapShotIndex;
    RBKit::AppState::getInstance()->setSnapshotName(snapShotIndex, snapshotName);
}

void SnapshotState::removeSnapshot(int index)
{
    if (index == 0)
        return;

    int tabSnapshotIndex = tabIndexToSnapshot[index];
    qDebug() << "- Removing snapshot " << tabSnapshotIndex << " : tab index " << index;
    if (heapForms.remove(tabSnapshotIndex) > 0) {
        qDebug() << "- successfully removed snapshot " << tabSnapshotIndex;
        tabIndexToSnapshot.remove(index);
        RBKit::AppState::getInstance()->removeSnapshotName(tabSnapshotIndex);
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

void SnapshotState::reset()
{
   snapShotInProgressFlag = false;
   snapShotIndex = 0;
   heapForms.clear();
   tabIndexToSnapshot.clear();
   tabIndexToSnapshot.append(0);
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

BaseHeapItem *SnapshotState::diffRootItem(QList<int> selectedSnapshots)
{
    BaseHeapItem *item1 = heapForms[selectedSnapshots.at(0)]->getRootItem();
    BaseHeapItem *item2 = heapForms[selectedSnapshots.at(1)]->getRootItem();

    return item2->minus(item1);
}

}
