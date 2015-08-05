#include "centralwidget.h"
#include "layoututil.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMessageBox>
#include <QStatusBar>
#include <QProgressBar>

#include "appmainwindow.h"
#include "memoryview.h"
#include "processdetail.h"
#include "heapdumpform.h"
#include "comparesnapshotform.h"
#include "diffviewform.h"
#include "model/appstate.h"
#include <typeinfo>
#include "model/cpustorage.h"

void disableCloseButtonOnFirstTab(QTabWidget *tabWidget) {
    QWidget *tabButton = tabWidget->tabBar()->tabButton(0, QTabBar::RightSide);
    if (tabButton) {
        tabButton->resize(0, 0);
    } else {
        tabWidget->tabBar()->tabButton(0, QTabBar::LeftSide)->resize(0, 0);
    }
}

QSharedPointer<RBKit::MemoryView> CentralWidget::getMemoryView() const
{
    return memoryView;
}

void CentralWidget::compareSnapshots()
{
    CompareSnapshotForm *compareSnapshots = new CompareSnapshotForm(this);
    QList<int> snapShotVersions = diffableSnapshotVersions();
    if (!snapShotVersions.isEmpty() && snapShotVersions.size() > 1) {
        compareSnapshots->setSnapshotVersions(snapShotVersions);
        connect(compareSnapshots, SIGNAL(snapshotSelected(QList<int>)),
                this, SLOT(onDiffSnapshotsSelected(QList<int>)));
    }

    compareSnapshots->show();
}

void CentralWidget::addTabWidget(HeapDumpForm *form, const QString &title)
{
    snapshotState->addNewSnapshot(HeapDumpFormPtr(form), title);
    form->setParentWindow(this);
    chartingTab->addTab(form, title);
}

void CentralWidget::appDisconnected()
{
    showStatusMessage("Not connected to any Ruby application");
    memoryView->processDetail->disconnectedFromProcess();
    snapshotState->reset();
    RBKit::SqlConnectionPool::getInstance()->closeDatabase();
    RBKit::SqlConnectionPool::getInstance()->setupDatabase();
}

void CentralWidget::onError(const QString &error)
{
    QMessageBox::critical(this, tr("rbkit"), error);
}

void CentralWidget::receiveYoungGenStats(QVariantMap map)
{
    memoryView->processDetail->receiveGenerationStats(0, map);
}

void CentralWidget::receiveSecondGenStats(QVariantMap map)
{
    memoryView->processDetail->receiveGenerationStats(1, map);
}

void CentralWidget::receiveOldGenStats(QVariantMap map)
{
    memoryView->processDetail->receiveGenerationStats(2, map);
}

void CentralWidget::objectDumpAvailable(int snapshotVersion)
{
    HeapDumpFormPtr heapUI = HeapDumpFormPtr(new HeapDumpForm(this, snapshotVersion));
    heapUI->setParentWindow(this);
    heapUI->loaData();
    QString snapshotName = QString("Heap Dump #%0").arg(snapshotVersion);
    snapshotState->addNewSnapshot(heapUI, snapshotName);
    showStatusMessage("Heap snapshot complete");
    //mainWindow->appStatusBar->clearMessage();
    setProgressBarValue(100);
    snapshotProgressTimer->stop();
    snapshotState->setSnapshotProgress(false);
    chartingTab->addTab(heapUI.data(), snapshotName);
}

void CentralWidget::updateProgressBar()
{
    if (snapshotState->snapShotInProgress()) {
        int currentProgress = RBKit::AppState::getInstance()->getState("heap_snapshot").toInt();
        setProgressBarValue(currentProgress);
    }
}

void CentralWidget::tabClosed(int index)
{
    if (index == 0)
        return;
    chartingTab->removeTab(index);
    snapshotState->removeSnapshot(index);
    cpuViewHash.remove(index);
}

void CentralWidget::onDiffSnapshotsSelected(QList<int> selectedSnapshots)
{
    RBKit::BaseHeapItem *newRootItem = snapshotState->diffRootItem(selectedSnapshots);

    DiffViewForm *form = new DiffViewForm(this, -1);
    form->setSnapshotDiffNumbers(selectedSnapshots);
    form->setContextDetail(RbkitContextDetail::ONLY_FILE);
    form->loadFromSpecifiedRoot(newRootItem);
    addTabWidget(form, QString("Compare Snapshots"));
}

QList<int> CentralWidget::diffableSnapshotVersions()
{
    return snapshotState->diffableSnapshotVersions();
}

CentralWidget::CentralWidget(QStackedWidget *window) : QWidget(window)
  , mainWindow(window)
{
    mainLayout = new QVBoxLayout();

    snapshotProgressTimer = new QTimer(this);
    snapshotState = new RBKit::SnapshotState();

    connect(snapshotProgressTimer, SIGNAL(timeout()),
            this, SLOT(updateProgressBar()));

    chartingTab = QSharedPointer<QTabWidget>::create(this);
    chartingTab->setTabsClosable(true);
    chartingTab->setMinimumHeight(700);
    chartingTab->setMinimumWidth(1100);

    mainLayout->addWidget(chartingTab.data());
    connect(chartingTab.data(), &QTabWidget::tabCloseRequested, this, &CentralWidget::tabClosed);
    setupCentralView();

    disableCloseButtonOnFirstTab(chartingTab.data());
    makeMarginSpacingZero(mainLayout);
    setLayout(mainLayout);
    //actionToolBar->disableProfileActions();
}

CentralWidget::~CentralWidget()
{
    delete mainLayout;
}

void CentralWidget::setupCentralView()
{
    qDebug() << "Adding object charts tab";
    memoryView = QSharedPointer<RBKit::MemoryView>::create(this);
    chartingTab->addTab(memoryView.data(), "Object Charts");
}

void CentralWidget::newCpuView()
{
    QSharedPointer<CpuView> cpuView(new CpuView(this));
    int index = chartingTab->addTab(cpuView.data(), "Cpu Tree");
    cpuViewHash[index] = cpuView;

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

void CentralWidget::showStatusMessage(const QString &message) const
{
    //mainWindow->appStatusBar->showMessage(message);
}

void CentralWidget::setProgressBarValue(int value) const
{
    //mainWindow->progressBar->setValue(value);
}

void CentralWidget::resetProgressBar() const
{
    //mainWindow->progressBar->reset();
}

bool CentralWidget::attemptMemorySnapshot()
{
    if (snapshotState->snapShotInProgress()) {
        QMessageBox alert(this);
        alert.setText("A snapshot is already in progress");
        alert.exec();
        return false;
    } else {
        // set heapsnapshot percentage to 2%
        RBKit::AppState::getInstance()->setAppState("heap_snapshot", 2);
        showStatusMessage("Snapshot Started");
        resetProgressBar();
        setProgressBarValue(2);
        snapshotProgressTimer->start(500);
        snapshotState->setSnapshotProgress(true);
        return true;
    }
}
