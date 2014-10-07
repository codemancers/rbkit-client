#ifndef DIFFVIEWFORM_H
#define DIFFVIEWFORM_H

#include "heapdumpform.h"
#include <QWidget>

class DiffViewForm : public HeapDumpForm
{
public:
    DiffViewForm(QWidget *parent = 0, int _snapShotVersion = 0);
    void loadFromSpecifiedRoot(RBKit::HeapItem *_rootItem);
public slots:
    void treeNodeSelected(const QModelIndex& index);
};

#endif // DIFFVIEWFORM_H
