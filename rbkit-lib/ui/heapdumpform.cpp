#include "heapdumpform.h"
#include "ui_heapdumpform.h"
#include "model/heap_item_types/heapitem.h"
#include "rbkitmainwindow.h"
#include <QStatusBar>

RbkitMainWindow *HeapDumpForm::getParentWindow() const
{
    return parentWindow;
}

void HeapDumpForm::setParentWindow(RbkitMainWindow *value)
{
    parentWindow = value;
}


RBKit::BaseHeapItem *HeapDumpForm::getRootItem() const
{
    return rootItem;
}

HeapDumpForm::HeapDumpForm(QWidget* parent, int _snapShotVersion)
    : QWidget(parent), ui(new Ui::HeapDumpForm), snapShotVersion(_snapShotVersion), disableRightClick(false)
{
    selecteItem = NULL;
    ui->setupUi(this);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setMouseTracking(true);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(treeNodeSelected(QModelIndex)));

    viewRefAct = new QAction(tr("View References"), this);
    viewRefAct->setStatusTip("View object references");
    connect(viewRefAct, SIGNAL(triggered()), this, SLOT(viewReferences()));
}

HeapDumpForm::~HeapDumpForm()
{
    delete ui;
    delete model;
    delete rootItem;
}

void HeapDumpForm::setDisableRightClick(bool value) {
    disableRightClick = value;
}

bool HeapDumpForm::getDisableRightClick() const {
    return disableRightClick;
}

void HeapDumpForm::setTreeModel(SortObjectProxyModel *model)
{
    ui->treeView->setModel(model);
}

void HeapDumpForm::loaData()
{
    rootItem = RBKit::SqlConnectionPool::getInstance()->rootOfSnapshot(snapShotVersion);
    model = new RBKit::HeapDataModel(rootItem, this);
    proxyModel = new SortObjectProxyModel(this);
    proxyModel->setSourceModel(model);
    setTreeModel(proxyModel);
    adjustColumnWidth();
}

void HeapDumpForm::loadSelectedReferences(RBKit::BaseHeapItem *_selectedItem)
{
    rootItem = _selectedItem->getSelectedReferences();
    model = new RBKit::HeapDataModel(rootItem, this);
    proxyModel = new SortObjectProxyModel(this);
    proxyModel->setSourceModel(model);
    setTreeModel(proxyModel);
    adjustColumnWidth();
}

void HeapDumpForm::adjustColumnWidth()
{
    ui->treeView->setSortingEnabled(true);
    ui->treeView->setColumnWidth(0, 300);
    ui->treeView->setColumnWidth(1, 100);
    ui->treeView->setColumnWidth(2, 180);
    ui->treeView->setColumnWidth(3, 180);
    ui->treeView->setColumnWidth(4, 180);
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->sortByColumn(1, Qt::DescendingOrder);
    connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(treeNodeSelected(QModelIndex)));
}

void HeapDumpForm::onCustomContextMenu(const QPoint &point)
{
    if (disableRightClick)
        return;
    QPoint localPoint = ui->treeView->viewport()->mapToGlobal(point);
    QModelIndex index = proxyModel->mapToSource(ui->treeView->indexAt(point));
    if (index.isValid()) {
        selecteItem = static_cast<RBKit::BaseHeapItem *>(index.internalPointer());
        QMenu menu(this);
        menu.addAction(viewRefAct);
        menu.exec(localPoint);
    }
}

void HeapDumpForm::viewReferences()
{
    HeapDumpForm *form = new HeapDumpForm(this, 0);
    form->setDisableRightClick(true);
    form->loadSelectedReferences(selecteItem);
    parentWindow->addTabWidget(form, QString("References for : %0").arg(selecteItem->shortLeadingIdentifier()));
}

void HeapDumpForm::treeNodeSelected(const QModelIndex &index)
{
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    RBKit::BaseHeapItem *nodeItem = static_cast<RBKit::BaseHeapItem *>(sourceIndex.internalPointer());
    if (nodeItem != NULL)
        parentWindow->statusBar()->showMessage(nodeItem->leadingIdentifier());
}


