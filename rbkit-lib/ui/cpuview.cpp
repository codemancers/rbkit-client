#include "cpuview.h"
#include "ui_cpuview.h"

CpuView::CpuView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CpuView)
{
    model = new QStandardItemModel;
    rootNode = model->invisibleRootItem();
    ui->setupUi(this);
}

CpuView::~CpuView()
{
    delete ui;
}
