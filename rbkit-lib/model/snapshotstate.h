#ifndef SNAPSHOTSTATE_H
#define SNAPSHOTSTATE_H

#include <QHash>
#include <QMap>
#include <QList>
#include <QSharedPointer>

#include "ui/heapdumpform.h"

namespace RBKit {

class BaseHeapItem;

class SnapshotState
{
    QMap<int, HeapDumpFormPtr> heapForms;
    int snapShotIndex;
    bool snapShotInProgressFlag;
    QVector<int> tabIndexToSnapshot;
public:
    SnapshotState();
    void addNewSnapshot(HeapDumpFormPtr form, QString snapshotName);
    void removeSnapshot(int index);
    bool snapShotInProgress() const;
    void setSnapshotProgress(bool snapShotState);
    void reset();
    QList<int> diffableSnapshotVersions();
    BaseHeapItem *diffRootItem(QList<int> selectedSnapshots);
    int getSnapShotIndex() const;
    void setSnapShotIndex(int value);
    QMap<int, HeapDumpFormPtr> getHeapForms() const;
    void setHeapForms(const QMap<int, HeapDumpFormPtr> &value);
    QVector<int> getTabIndexToSnapshot() const;
    void setTabIndexToSnapshot(const QVector<int> &value);
};
}


#endif // SNAPSHOTSTATE_H
