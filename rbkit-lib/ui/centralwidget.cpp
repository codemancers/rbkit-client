#include "centralwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>

static void makeMarginSpacingZero(QBoxLayout *layout) {
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout();
    actionToolBar = new ActionToolbar(this);
    mainLayout->addWidget(actionToolBar->getToolBar(), 0, Qt::AlignTop);

    chartingTab = new QTabWidget(this);
    chartingTab->setMinimumHeight(840);
    chartingTab->setMinimumWidth(1100);
    mainLayout->addWidget(chartingTab, 0, Qt::AlignTop);

    makeMarginSpacingZero(mainLayout);
    setLayout(mainLayout);
}

CentralWidget::~CentralWidget()
{

}

