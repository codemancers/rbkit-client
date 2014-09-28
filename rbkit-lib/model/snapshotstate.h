#ifndef SNAPSHOTSTATE_H
#define SNAPSHOTSTATE_H

#include <QHash>
#include <QMap>
#include <QList>
#include "ui/heapdumpform.h"

class SnapshotState
{
    QMap<int, HeapDumpForm *> heapForms;
    int snapShotIndex;
public:
    SnapshotState();
    void addNewSnapshot(HeapDumpForm *form);
    void removeSnapshot(int index);
    bool snapShotInProgress() const;
    int getSnapshotProgress() const;
    void setSnapshotProgress();
    QList<int> diffableSnapshotVersions();
};

#endif // SNAPSHOTSTATE_H
