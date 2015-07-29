#include "cpuview.h"
#include "ui_cpuview.h"
#include <QDebug>

CpuView::CpuView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CpuView)
{
    ui->setupUi(this);
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
