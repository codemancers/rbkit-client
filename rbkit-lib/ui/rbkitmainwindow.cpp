#include "rbkitmainwindow.h"
#include "ui_rbkitmainwindow.h"
#include "askhost.h"
#include "model/jsbridge.h"
#include "model/appstate.h"
#include "comapresnapshotform.h"
#include "diffviewform.h"
#include "ui/actiontoolbar.h"
#include "ui/aboutdialog.h"
#include "rbheapworker.h"


RbkitMainWindow::RbkitMainWindow(QWidget *parent) :
    QMainWindow(parent), connected(false), host(""), connectionInProgress(false),
    ui(new Ui::RbkitMainWindow)
{
    Q_INIT_RESOURCE(tool_icons);
    RBKit::SqlConnectionPool::getInstance()->setupDatabase();
    ui->setupUi(this);
    actionToolbar = new ActionToolbar(ui);
    setupToolbarStyle();

    snapshotProgressTimer = new QTimer(this);
    snapShotState = new RBKit::SnapshotState();

    connect(snapshotProgressTimer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));

    statusLabel = new QLabel(this);
    progressBar = new QProgressBar(this);

    statusLabel->setText("Snapshot Progress");
    progressBar->setTextVisible(false);
    ui->statusbar->addWidget(statusLabel);
    ui->statusbar->addWidget(progressBar);

    qRegisterMetaType<RBKit::ObjectStore>();
    qRegisterMetaType<RBKit::ObjectDetail>();
    memoryView = new RBKit::MemoryView();
    ui->chartingTab->addTab(memoryView, "Object Charts");
    connect(ui->chartingTab, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClosed(int)));
    disableCloseButtonOnFirstTab();
    actionToolbar->disableProfileActions();
}


RbkitMainWindow::~RbkitMainWindow()
{
    delete ui;
}

void RbkitMainWindow::addTabWidget(HeapDumpForm *heapDumpForm, const QString& title)
{
    snapShotState->addNewSnapshot(HeapDumpFormPtr(heapDumpForm), title);
    heapDumpForm->setParentWindow(this);
    ui->chartingTab->addTab(heapDumpForm, title);
}

void RbkitMainWindow::on_action_Connect_triggered()
{
    qDebug() << "main-window" << connected;

    if (!connectionInProgress) {
        setupSubscriber();
        askForServerInfo();
    } else {
        disconnectFromSocket();
    }
}

void RbkitMainWindow::on_action_Quit_triggered()
{
    disconnectFromSocket();
    close();
}

void RbkitMainWindow::askForServerInfo() {
    if(host.size() == 0) {
        this->askHost = new AskHost(this);
        connect(this->askHost, SIGNAL(userHasSelectedHost(QString, QString)),
                this, SLOT(useSelectedHost(QString, QString)));
        askHost->show();
    }
}

void RbkitMainWindow::setupToolbarStyle()
{
    ui->toolBar->setStyleSheet("QToolButton:pressed { background-color: rgb(160,160,160); border-style: inset; }");
}

QList<int> RbkitMainWindow::diffableSnapshotVersions()
{
    return snapShotState->diffableSnapshotVersions();
}

void RbkitMainWindow::disableCloseButtonOnFirstTab()
{
    QWidget *tabButton = ui->chartingTab->tabBar()->tabButton(0, QTabBar::RightSide);
    if (tabButton) {
        tabButton->resize(0, 0);
    } else {
        ui->chartingTab->tabBar()->tabButton(0, QTabBar::LeftSide)->resize(0, 0);
    }
}


void RbkitMainWindow::useSelectedHost(QString commandsSocket, QString eventsSocket)
{
    askHost->close();
    connectionInProgress = true;
    ui->action_Connect->setText(tr("&Disconnect"));
    ui->action_Connect->setIcon(QIcon(":/icons/disconnect-32.png"));
    emit connectToSocket(commandsSocket, eventsSocket);
}

void RbkitMainWindow::on_action_About_Rbkit_triggered()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->show();
}


void RbkitMainWindow::objectDumpAvailable(int snapshotVersion)
{
    HeapDumpFormPtr heapUI = HeapDumpFormPtr(new HeapDumpForm(this, snapshotVersion));
    heapUI->setParentWindow(this);
    heapUI->loaData();
    QString snapshotName = QString("Heap Dump #%0").arg(snapshotVersion);
    snapShotState->addNewSnapshot(heapUI, snapshotName);
    ui->statusbar->showMessage("Heap snapshot complete");
    ui->statusbar->clearMessage();
    progressBar->setValue(100);
    snapshotProgressTimer->stop();
    snapShotState->setSnapshotProgress(false);
    ui->chartingTab->addTab(heapUI.data(), snapshotName);
}

void RbkitMainWindow::disconnectFromSocket()
{
    qDebug() << "Attempting to stop the thread";
    subscriberThread.requestInterruption();
    subscriberThread.exit();
    subscriberThread.wait();
    qDebug() << "Thread has been stopped";
}

void RbkitMainWindow::setupSubscriber()
{
    //Create a subscriber and move it to it's own thread
    subscriber = new Subscriber(memoryView->getJsBridge());

    //Events to/from parent/subcriber thread
    connect(&subscriberThread, &QThread::finished, subscriber, &QObject::deleteLater);
    connect(&subscriberThread, SIGNAL(started()), subscriber, SLOT(startSubscriber()));
    connect(this, SIGNAL(connectToSocket(QString, QString)),
            subscriber, SLOT(startListening(QString, QString)));
    connect(this, SIGNAL(triggerGc()), subscriber, SLOT(triggerGc()));
    connect(this, SIGNAL(takeSnapshot()), subscriber, SLOT(takeSnapshot()));

    connect(subscriber, &Subscriber::errored, this, &RbkitMainWindow::onError);
    connect(subscriber, &Subscriber::connected, this, &RbkitMainWindow::connectedToSocket);
    connect(subscriber, &Subscriber::disconnected, this, &RbkitMainWindow::disconnectedFromSocket);


    // create db heap dumper, and connect subscriber to dumper.
    heapWorker.reset(new RBKit::RbHeapWorker());
    connect(subscriber, SIGNAL(dumpReceived(const QByteArray)),
            heapWorker.data(), SLOT(dump(const QByteArray)));
    connect(heapWorker.data(), SIGNAL(dumpAvailable(int)),
            this, SLOT(objectDumpAvailable(int)));

    subscriber->moveToThread(&subscriberThread);
    heapWorker->moveToThread(&heapDumpThread);

    subscriberThread.start();
    heapDumpThread.start();
}

void RbkitMainWindow::disconnectedFromSocket()
{
    ui->action_Connect->setText(tr("&Connect"));
    ui->action_Connect->setIcon(QIcon(":/icons/connect-32.png"));
    this->connected = false;
    ui->statusbar->showMessage("Not connected to any Ruby application");
    actionToolbar->disableProfileActions();
    connectionInProgress = false;
}


void RbkitMainWindow::connectedToSocket()
{
    actionToolbar->enableProfileActions();
    ui->action_Connect->setText(tr("&Disconnect"));
    ui->action_Connect->setIcon(QIcon(":/icons/disconnect-32.png"));
    ui->statusbar->showMessage("Currently Profiling Ruby application");
    this->connected = true;
}

void RbkitMainWindow::onError(const QString &error)
{
    disconnectFromSocket();
    QMessageBox::critical(this, tr("rbkit"), error);
}

void RbkitMainWindow::on_action_Trigger_GC_triggered()
{
    emit triggerGc();
}

void RbkitMainWindow::on_actionHeap_Snapshot_triggered()
{
    if (snapShotState->snapShotInProgress()) {
        QMessageBox alert(this);
        alert.setText("A snapshot is already in progress");
        alert.exec();
        return;
    } else {
        // set heapsnapshot percentage to 2%
        RBKit::AppState::getInstance()->setAppState("heap_snapshot", 2);
        ui->statusbar->showMessage("Snapshot started");
        ui->statusbar->clearMessage();
        progressBar->reset();
        progressBar->setValue(2);
        snapshotProgressTimer->start(500);
        snapShotState->setSnapshotProgress(true);
        emit takeSnapshot();
    }
}

void RbkitMainWindow::tabClosed(int index)
{
    if (index == 0)
        return;
    ui->chartingTab->removeTab(index);
    snapShotState->removeSnapshot(index);
}

void RbkitMainWindow::updateProgressBar()
{
    if (snapShotState->snapShotInProgress()) {
        int currentProgress = RBKit::AppState::getInstance()->getState("heap_snapshot").toInt();
        progressBar->setValue(currentProgress);
    }
}


void RbkitMainWindow::on_actionComapre_Heapsnapshots_triggered()
{
    ComapreSnapshotForm *compareSnapshots = new ComapreSnapshotForm(this);
    QList<int> snapShotVersions = diffableSnapshotVersions();
    if (!snapShotVersions.isEmpty() && snapShotVersions.size() > 1) {
        compareSnapshots->setSnapshotVersions(snapShotVersions);
        connect(compareSnapshots, SIGNAL(snapshotSelected(QList<int>)), this, SLOT(onDiffSnapshotsSelected(QList<int>)));
    }

    compareSnapshots->show();
}

void RbkitMainWindow::onDiffSnapshotsSelected(QList<int> selectedSnapshots)
{
    RBKit::BaseHeapItem *newRootItem = snapShotState->diffRootItem(selectedSnapshots);

    DiffViewForm *form = new DiffViewForm(this, -1);
    form->setDisableRightClick(true);
    form->loadFromSpecifiedRoot(newRootItem);
    addTabWidget(form, QString("Comapre Snapshots"));
}
