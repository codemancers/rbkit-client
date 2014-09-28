#ifndef SNAPSHOTSTATE_H
#define SNAPSHOTSTATE_H

#include <QHash>
#include <QMap>
#include <QList>
#include <QSharedPointer>

#include "ui/heapdumpform.h"

namespace RBKit {
class SnapshotState
{
    QMap<int, HeapDumpFormPtr> heapForms;
    int snapShotIndex;
    bool snapShotInProgressFlag;
public:
    SnapshotState();
    void addNewSnapshot(HeapDumpFormPtr form);
    void removeSnapshot(int index);
    bool snapShotInProgress() const;
    void setSnapshotProgress(bool snapShotState);
    QList<int> diffableSnapshotVersions();
};
}


#endif // SNAPSHOTSTATE_H
