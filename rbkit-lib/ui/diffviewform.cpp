#include "diffviewform.h"
#include "rbkitmainwindow.h"
#include "ui_heapdumpform.h"
#include "parentviewform.h"
#include "model/diffitem.h"
#include <QStatusBar>

DiffViewForm::DiffViewForm(QWidget* parent, int _snapShotVersion)
    : HeapDumpForm(parent, _snapShotVersion), parentViewForm(0)
{
}

void DiffViewForm::loadFromSpecifiedRoot(RBKit::DiffItem *_rootItem)
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
    qDebug() << "Calling this method here";
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    RBKit::HeapItem *nodeItem = static_cast<RBKit::HeapItem *>(sourceIndex.internalPointer());
    if (nodeItem != NULL) {
        parentWindow->statusBar()->showMessage(nodeItem->leadingIdentifier());
        if (parentViewForm == 0) {
            initializeParentView();
        }
        updateParentView(nodeItem);
    }
}

void DiffViewForm::updateParentView(RBKit::HeapItem *heapItem)
{
    parentViewForm->setLabel(heapItem->className);
}

void DiffViewForm::initializeParentView()
{
    parentViewForm = new ParentViewForm(this);
    ui->treeVerticalLayout->addWidget(parentViewForm);
    qDebug() << "Showing the parent form";
    parentViewForm->show();
}
