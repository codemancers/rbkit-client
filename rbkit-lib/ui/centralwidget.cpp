#include "centralwidget.h"
#include "layoututil.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>


CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout();
    actionToolBar = new ActionToolbar(this);
    mainLayout->addWidget(actionToolBar->getToolBar(), 0, Qt::AlignTop);

    chartingTab = new QTabWidget(this);
    chartingTab->setMinimumHeight(840);
    chartingTab->setMinimumWidth(1100);
    mainLayout->addWidget(chartingTab);

    makeMarginSpacingZero(mainLayout);
    setLayout(mainLayout);
}

CentralWidget::~CentralWidget()
{
    delete chartingTab;
    delete memoryView;
}

void CentralWidget::setupCentralView()
{
    memoryView = new RBKit::MemoryView();
    chartingTab->addTab(memoryView, "Object Charts");
}

