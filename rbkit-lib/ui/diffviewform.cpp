#include "diffviewform.h"
#include "rbkitmainwindow.h"
#include "ui_heapdumpform.h"
#include "parentviewform.h"
#include "model/heap_item_types/baseheapitem.h"
#include <QStatusBar>

DiffViewForm::DiffViewForm(QWidget* parent, int _snapShotVersion)
    : HeapDumpForm(parent, _snapShotVersion), parentViewForm(0)
{
}

DiffViewForm::~DiffViewForm()
{
    if (parentViewForm != NULL)  {
        qDebug() << "Delete parent view form";
        delete parentViewForm;
    }
}

void DiffViewForm::treeNodeSelected(const QModelIndex &index)
{
    qDebug() << "Calling this method here";
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    RBKit::BaseHeapItem *nodeItem = static_cast<RBKit::BaseHeapItem *>(sourceIndex.internalPointer());
    if (nodeItem != NULL) {
        parentWindow->statusBar()->showMessage(nodeItem->leadingIdentifier());
        if (parentViewForm == 0) {
            initializeParentView();
        }
        updateParentView(nodeItem);
    }
}

void DiffViewForm::updateParentView(RBKit::BaseHeapItem *heapItem)
{
    parentViewForm->reset();
    RBKit::BaseHeapItem *parentHeapItem = rootItem->getObjectParents(heapItem);
    parentViewForm->loadFromSpecifiedRoot(parentHeapItem);
}

void DiffViewForm::initializeParentView()
{
    parentViewForm = new HeapDumpForm(this, -1);
    parentViewForm->setDisableRightClick(true);
    parentViewForm->setParentWindow(getParentWindow());
    ui->treeVerticalLayout->addWidget(parentViewForm);
    qDebug() << "Showing the parent form";
    parentViewForm->show();
}
