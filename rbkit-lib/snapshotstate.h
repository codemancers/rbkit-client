#ifndef SNAPSHOTSTATE_H
#define SNAPSHOTSTATE_H

#include <QHash>
#include <QMap>
#include "heapdumpform.h"

class SnapshotState
{
    QMap<int, HeapDumpForm *> heapForms;
    int snapShotIndex;
public:
    SnapshotState();
    void addNewSnapshot(HeapDumpForm *form);
    void removeSnapshot(int index);
    bool snapShotInProgress();
    int getSnapshotProgress();
    void setSnapshotProgress();
};

#endif // SNAPSHOTSTATE_H
