#include "heapdumpform.h"
#include "ui_heapdumpform.h"

HeapDumpForm::HeapDumpForm(QWidget *parent, int _snapShotVersion) :
    QWidget(parent),
    ui(new Ui::HeapDumpForm), snapShotVersion(_snapShotVersion)
{
    selecteItem = NULL;
    ui->setupUi(this);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    viewRefAct = new QAction(tr("View References"), this);
    viewRefAct->setStatusTip("View object references");
    connect(viewRefAct, SIGNAL(triggered()), this, SLOT(viewReferences()));
}

HeapDumpForm::~HeapDumpForm()
{
    delete ui;
    delete proxyModel;
    delete model;
    delete rootItem;
}

void HeapDumpForm::loaData()
{
    rootItem = RBKit::SqlConnectionPool::getInstance()->rootOfSnapshot(snapShotVersion);
    model = new RBKit::HeapDataModel(rootItem, this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->treeView->setModel(proxyModel);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->setColumnWidth(0, 300);
    ui->treeView->setColumnWidth(1, 100);
    ui->treeView->setColumnWidth(2, 180);
    ui->treeView->setColumnWidth(3, 180);
    ui->treeView->setColumnWidth(4, 180);
}

void HeapDumpForm::loadSelectedReferences(RBKit::HeapItem *_selectedItem)
{

}

void HeapDumpForm::onCustomContextMenu(const QPoint &point)
{
    QPoint localPoint = ui->treeView->viewport()->mapToGlobal(point);
    QModelIndex index = proxyModel->mapToSource(ui->treeView->indexAt(point));
    if (index.isValid()) {
        selecteItem = static_cast<RBKit::HeapItem *>(index.internalPointer());
        QMenu menu(this);
        menu.addAction(viewRefAct);
        menu.exec(localPoint);
    }
}

void HeapDumpForm::viewReferences()
{
    RbkitMainWindow *window = static_cast<RbkitMainWindow *>(parentWidget());
    HeapDumpForm *form = new HeapDumpForm(window, 0);
    form->loadSelectedReferences(selecteItem);
    window->addTabWidget(form, "References for");
}


