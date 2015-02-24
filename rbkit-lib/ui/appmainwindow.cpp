#include "appmainwindow.h"

AppMainwindow::AppMainwindow(QWidget *parent) : QMainWindow(parent)
{
    centralWidget = new CentralWidget(this);
    setCentralWidget(centralWidget);
}

AppMainwindow::~AppMainwindow()
{
    delete centralWidget;
}

