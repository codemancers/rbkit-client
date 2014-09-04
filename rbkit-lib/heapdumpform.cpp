#include "heapdumpform.h"
#include "ui_heapdumpform.h"

HeapDumpForm::HeapDumpForm(QWidget *parent, int _snapShotVersion) :
    QWidget(parent),
    ui(new Ui::HeapDumpForm), snapShotVersion(_snapShotVersion)
{
    ui->setupUi(this);
}

HeapDumpForm::~HeapDumpForm()
{
    delete ui;
}

void HeapDumpForm::loaData()
{
    RBKit::HeapItem *rootItem = RBKit::SqlConnectionPool::getInstance()->rootOfSnapshot(snapShotVersion);
    RBKit::HeapDataModel *model = new RBKit::HeapDataModel(rootItem, this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->treeView->setModel(proxyModel);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->setColumnWidth(0, 300);
    ui->treeView->setColumnWidth(1, 150);
    ui->treeView->setColumnWidth(2, 200);
    ui->treeView->setColumnWidth(3, 200);
    ui->treeView->setColumnWidth(4, 200);
}


