#include "appmainwindow.h"

#include <QStatusBar>

AppMainwindow::AppMainwindow(QWidget *parent) : QMainWindow(parent)
{
    centralWidget = new CentralWidget(this);
    setCentralWidget(centralWidget);

    appStatusBar = new QStatusBar(this);
    setStatusBar(appStatusBar);
}

AppMainwindow::~AppMainwindow()
{
    delete centralWidget;
}

