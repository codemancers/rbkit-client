#ifndef DIFFVIEWFORM_H
#define DIFFVIEWFORM_H

#include "heapdumpform.h"
#include <QWidget>
#include "parentviewform.h"

class DiffViewForm : public HeapDumpForm
{
    HeapDumpForm *parentViewForm;
public:
    DiffViewForm(QWidget *parent = 0, int _snapShotVersion = 0);
public slots:
    virtual void treeNodeSelected(const QModelIndex& index);
    void updateParentView(RBKit::BaseHeapItem *heapItem);
    void initializeParentView();
};

#endif // DIFFVIEWFORM_H
