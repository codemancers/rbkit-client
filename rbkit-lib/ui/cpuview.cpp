#include "cpuview.h"
#include "ui_cpuview.h"
#include <QDebug>
#include "model/cpustorage.h"

CpuView::CpuView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CpuView)
{
    callGraphModel = new QStandardItemModel;
    flatGraphModel = new QStandardItemModel;

    ui->setupUi(this);

    // column headers for each model
    QStringList headers;
    headers.append("Methods");
    headers.append("Self");
    headers.append("Total");
    this->callGraphModel->setHorizontalHeaderLabels(headers);
    this->flatGraphModel->setHorizontalHeaderLabels(headers);

    // setting callgraph as default view
    updateCpuModel(callGraphModel);
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
    updateCpuModel(callGraphModel);
}

void CpuView::on_flatRadio_clicked()
{
    updateCpuModel(flatGraphModel);
}
