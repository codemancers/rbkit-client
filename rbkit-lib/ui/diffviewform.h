#ifndef DIFFVIEWFORM_H
#define DIFFVIEWFORM_H

#include "heapdumpform.h"
#include <QWidget>
#include "parentviewform.h"
#include <QLabel>

class DiffViewForm : public HeapDumpForm
{
    HeapDumpForm *parentViewForm;
    QLabel *diffLabel;
    QLabel *parentLabel;
public:
    DiffViewForm(QWidget *parent = 0, int _snapShotVersion = 0);
    virtual ~DiffViewForm();
public slots:
    virtual void treeNodeSelected(const QModelIndex& index);
    void updateParentView(RBKit::BaseHeapItem *heapItem);
    void initializeParentView();
    void setSnapshotDiffNumbers(QList<int> selectedSnapshots);
};

#endif // DIFFVIEWFORM_H
