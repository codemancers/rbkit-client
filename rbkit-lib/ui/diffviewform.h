#ifndef DIFFVIEWFORM_H
#define DIFFVIEWFORM_H

#include "heapdumpform.h"
#include <QWidget>
#include "parentviewform.h"

namespace RBKit {
class DiffItem;
}

class DiffViewForm : public HeapDumpForm
{
    ParentViewForm *parentViewForm;
public:
    DiffViewForm(QWidget *parent = 0, int _snapShotVersion = 0);
    void loadFromSpecifiedRoot(RBKit::DiffItem *_rootItem);
public slots:
    virtual void treeNodeSelected(const QModelIndex& index);
    void updateParentView();
    void initializeParentView();
};

#endif // DIFFVIEWFORM_H
