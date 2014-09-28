#include "diffviewform.h"

DiffViewForm::DiffViewForm(QWidget* parent, int _snapShotVersion)
    : HeapDumpForm(parent, _snapShotVersion)
{
}

void DiffViewForm::loadFromSpecifiedRoot(RBKit::HeapItem *_rootItem)
{
    rootItem = _rootItem;
    model = new RBKit::HeapDataModel(rootItem, this);
    proxyModel = new SortObjectProxyModel(this);
    proxyModel->setSourceModel(model);
    setTreeModel(proxyModel);
    adjustColumnWidth();
}
