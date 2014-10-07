#include "diffviewform.h"
#include "rbkitmainwindow.h"
#include <QStatusBar>

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

void DiffViewForm::treeNodeSelected(const QModelIndex &index)
{
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    RBKit::HeapItem *nodeItem = static_cast<RBKit::HeapItem *>(sourceIndex.internalPointer());
    if (nodeItem != NULL)
        parentWindow->statusBar()->showMessage(nodeItem->leadingIdentifier());
}
