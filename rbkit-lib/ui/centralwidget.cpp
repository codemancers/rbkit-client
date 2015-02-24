#include "centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
}

CentralWidget::~CentralWidget()
{

}

