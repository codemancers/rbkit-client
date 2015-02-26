#include "centralwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout();
    actionToolBar = new ActionToolbar(this);
    mainLayout->addWidget(actionToolBar->getToolBar(), 0, Qt::AlignTop);
    setLayout(mainLayout);
}

CentralWidget::~CentralWidget()
{

}

