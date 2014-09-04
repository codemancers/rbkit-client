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
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
}


