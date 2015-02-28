#include "centralwidget.h"
#include "layoututil.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>

QSharedPointer<RBKit::MemoryView> CentralWidget::getMemoryView() const
{
    return memoryView;
}

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout();
    actionToolBar = QSharedPointer<ActionToolbar>::create(this);
    mainLayout->addWidget(actionToolBar->getToolBar(), 0, Qt::AlignTop);

    chartingTab = QSharedPointer<QTabWidget>::create(this);
    chartingTab->setMinimumHeight(700);
    chartingTab->setMinimumWidth(1100);
    mainLayout->addWidget(chartingTab);

    makeMarginSpacingZero(mainLayout);
    setLayout(mainLayout);
}

CentralWidget::~CentralWidget()
{
    delete mainLayout;
}

void CentralWidget::setupCentralView()
{
    memoryView = QSharedPointer<RBKit::MemoryView>::create();
    chartingTab->addTab(memoryView, "Object Charts");
}

