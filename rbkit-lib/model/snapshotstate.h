#ifndef SNAPSHOTSTATE_H
#define SNAPSHOTSTATE_H

#include <QHash>
#include <QMap>
#include <QList>
#include <QSharedPointer>

#include "ui/heapdumpform.h"
#include "model/heapitem.h"

namespace RBKit {
class SnapshotState
{
    QMap<int, HeapDumpFormPtr> heapForms;
    int snapShotIndex;
    bool snapShotInProgressFlag;
public:
    SnapshotState();
    void addNewSnapshot(HeapDumpFormPtr form, QString snapshotName);
    void removeSnapshot(int index);
    bool snapShotInProgress() const;
    void setSnapshotProgress(bool snapShotState);
    QList<int> diffableSnapshotVersions();
    HeapItem *diffRootItem(QList<int> selectedSnapshots);
};
}


#endif // SNAPSHOTSTATE_H
