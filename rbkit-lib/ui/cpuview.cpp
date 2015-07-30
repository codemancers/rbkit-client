#include "cpuview.h"
#include "ui_cpuview.h"
#include <QDebug>
#include "model/cpustorage.h"

CpuView::CpuView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CpuView)
{
    ui->setupUi(this);
    connect(this, SIGNAL(traverseCallGraph()), RBKit::CpuStorage::getStorage().data(), SLOT(changeToCallGraph()));
    connect(this, SIGNAL(traverseFlatProfile()), RBKit::CpuStorage::getStorage().data(), SLOT(changeToFlatProfile()));
}

CpuView::~CpuView()
{
    delete ui;
}

void CpuView::updateCpuModel(QStandardItemModel *model)
{
    qDebug() << "inside cpu tree signal";
    ui->cpuTree->setModel(model);
    ui->cpuTree->show();
}

void CpuView::on_callGraphRadio_clicked()
{
    emit traverseCallGraph();
}

void CpuView::on_flatRadio_clicked()
{
    emit traverseFlatProfile();
}
