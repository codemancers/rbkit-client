#include "appmainwindow.h"

#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>
#include "model/objectdetail.h"
#include "model/objectstore.h"

AppMainwindow::AppMainwindow(QWidget *parent) : QMainWindow(parent)
{
    stackedWidget = new StackedWidget(this);
    setCentralWidget(stackedWidget);

    centralMemoryWidget = new CentralWidget(stackedWidget);
    //setCentralWidget(centralWidget);
    int index = stackedWidget->addWidget(centralMemoryWidget);

    stackedWidget->setCurrentIndex(index);

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

/*CpuViewPtr AppMainwindow::getCpuView()
{
    return centralWidget->getCpuViewPtr();
}*/

AppMainwindow::~AppMainwindow()
{
    delete centralWidget;
}

void AppMainwindow::tabChanged(int tab)
{
    stackedWidget->setCurrentIndex(tab);
}
