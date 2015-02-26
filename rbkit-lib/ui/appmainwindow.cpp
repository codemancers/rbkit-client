#include "appmainwindow.h"

#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>

AppMainwindow::AppMainwindow(QWidget *parent) : QMainWindow(parent)
{
    centralWidget = new CentralWidget(this);
    setCentralWidget(centralWidget);

    appStatusBar = new QStatusBar(this);
    setStatusBar(appStatusBar);

    statusLabel = new QLabel(this);
    progressBar = new QProgressBar(this);

    statusLabel->setText("Snapshot Progress");
    progressBar->setTextVisible(false);
    appStatusBar->addWidget(statusLabel);
    appStatusBar->addWidget(progressBar);

    qRegisterMetaType<RBKit::ObjectStore>();
    qRegisterMetaType<RBKit::ObjectDetail>();
}

AppMainwindow::~AppMainwindow()
{
    delete centralWidget;
}

