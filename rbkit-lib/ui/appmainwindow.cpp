#include "appmainwindow.h"

#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>
#include "model/objectdetail.h"
#include "model/objectstore.h"

AppMainwindow::AppMainwindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget(this);

    mainLayout = new QVBoxLayout();

    stackedWidget = new StackedWidget(this);
    setCentralWidget(mainWidget);

    centralMemoryWidget = new CentralWidget(stackedWidget, this);
    int index = stackedWidget->addWidget(centralMemoryWidget);

    actionToolBar = QSharedPointer<ActionToolbar>::create(*this, centralMemoryWidget);

    cpuTab = new QTabWidget(stackedWidget);
    cpuTab->setTabsClosable(true);
    connect(cpuTab,
            SIGNAL(tabCloseRequested(int)),
            this,
            SLOT(closeCpuTab(int)));

    stackedWidget->addWidget(cpuTab);

    stackedWidget->setCurrentIndex(index);

    actionToolBar.data()->connectTabChangedSignal(this);

    mainLayout->addWidget(actionToolBar.data()->getToolBar(), 0, Qt::AlignTop);
    mainLayout->addWidget(stackedWidget, 1);

    mainWidget->setLayout(mainLayout);
    mainWidget->show();

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

void AppMainwindow::newCpuView()
{
    QSharedPointer<CpuView> cpuView(new CpuView());
    cpuTab->addTab(cpuView.data(), "Cpu Tree");
    cpuViewList.append(cpuView);

    connect(cpuView.data(),
            SIGNAL(fillCallGraph(QStandardItemModel*)),
            RBKit::CpuStorage::getStorage().data(),
            SLOT(fillCallGraphModel(QStandardItemModel*)));

    connect(cpuView.data(),
            SIGNAL(fillFlatProfile(QStandardItemModel*)),
            RBKit::CpuStorage::getStorage().data(),
            SLOT(fillFlatProfileModel(QStandardItemModel*)));

    emit cpuView.data()->fillCallGraph(cpuView->callGraphModel);
    emit cpuView.data()->fillFlatProfile(cpuView->flatGraphModel);
}

AppMainwindow::~AppMainwindow()
{
    delete centralMemoryWidget;
}

void AppMainwindow::tabChanged(int tab)
{
    stackedWidget->setCurrentIndex(tab);
}

void AppMainwindow::closeCpuTab(int index)
{
    cpuTab->removeTab(index);
    cpuViewList.removeAt(index);
}
