#include "actiontoolbar.h"

#include <QIcon>
#include <QToolButton>
#include <QStatusBar>
#include <QMessageBox>

#include "centralwidget.h"
#include "memoryview.h"

static void toggleConnectButtonState(ConnectionStates state, QToolButton *btn) {
    switch (state) {
    case CONNECTED:
        btn->setText(tr("&Disconnect"));
        btn->setIcon(QIcon(":/icons/disconnect-32.png"));
        break;
    case DISCONNECTED:
        btn->setText(tr("&Connect"));
        btn->setIcon(QIcon(":/icons/connect-32.png"));
        break;
    default:
        btn->setText(tr("&Disconnect"));
        btn->setIcon(QIcon(":/icons/disconnect-32.png"));
        break;
    }
}

ActionToolbar::ActionToolbar(CentralWidget *widget)
    : centralWidget(widget)
{
    Q_INIT_RESOURCE(tool_icons);
    connectionState = DISCONNECTED;
    setupToolBar();
}

void ActionToolbar::enableProfileActions()
{
    gcButton->setEnabled(true);
    compareSnapshotButton->setEnabled(true);
    snapshotButton->setEnabled(true);
    gcButton->setEnabled(true);
}

void ActionToolbar::disableProfileActions()
{
    gcButton->setDisabled(true);
    compareSnapshotButton->setDisabled(true);
    snapshotButton->setDisabled(true);
    gcButton->setDisabled(true);
}

RibbonToolBar *ActionToolbar::getToolBar() const
{
    return toolBar;
}

void ActionToolbar::setupSubscriber()
{
    //Create a subscriber and move it to it's own thread
    subscriber = new Subscriber(memoryView()->getJsBridge());
    subscriber->moveToThread(&subscriberThread);

    //Events to/from parent/subcriber thread
    connect(&subscriberThread, &QThread::finished, subscriber, &QObject::deleteLater);
    connect(&subscriberThread, SIGNAL(started()), subscriber, SLOT(startSubscriber()));
    connect(this, SIGNAL(connectToSocket(QString, QString)),
            subscriber, SLOT(startListening(QString, QString)));
    connect(this, SIGNAL(triggerGc()), subscriber, SLOT(triggerGc()));
    connect(this, SIGNAL(takeSnapshot()), subscriber, SLOT(takeSnapshot()));

    connect(subscriber, &Subscriber::errored, centralWidget, &CentralWidget::onError);
    connect(subscriber, &Subscriber::connected, this, &RbkitMainWindow::connectedToSocket);
    connect(subscriber, &Subscriber::disconnected, this, &RbkitMainWindow::disconnectedFromSocket);
    connect(subscriber, &Subscriber::objectDumpAvailable, this, &RbkitMainWindow::objectDumpAvailable);

    connect(subscriber, &Subscriber::youngGenStats, this, &RbkitMainWindow::receiveYoungGenStats);
    connect(subscriber, &Subscriber::secondGenStats, this, &RbkitMainWindow::receiveSecondGenStats);
    connect(subscriber, &Subscriber::oldGenStats, this, &RbkitMainWindow::receiveOldGenStats);

    subscriberThread.start();
}

void ActionToolbar::askForServerInfo()
{
    if(host.size() == 0) {
        askHost = new AskHost(this);
        connect(askHost, SIGNAL(userHasSelectedHost(QString, QString)),
                this, SLOT(useSelectedHost(QString, QString)));
        askHost->show();
    }
}

RBKit::MemoryView *ActionToolbar::memoryView() const
{
    centralWidget->getMemoryView().data();
}

void ActionToolbar::disconnectFromSocket()
{
    qDebug() << "Attempting to stop the thread";
    subscriberThread.requestInterruption();
    subscriberThread.exit();
    subscriberThread.wait();
    qDebug() << "Thread has been stopped";
}

void ActionToolbar::setupToolBar()
{
    toolBar = new RibbonToolBar(centralWidget);
    connectButton = toolBar->addRibbonAction("Connect", "connect_action", QIcon(":/icons/connect-32.png"));
    connect(connectButton, &QToolButton::clicked, this, &ActionToolbar::attemptConnection);


    toolBar->addRibbonTab("CPU Profiling", "cpu_profiling");
    toolBar->addRibbonTab("Memory Profiling", "memory_profiling");

    compareSnapshotButton = toolBar->addRibbonAction("Compare Snapshot",
                                                     "compare_snapshot",
                                                     QIcon(":/icons/Compare-32.png"),
                                                     "memory_profiling");
    connect(compareSnapshotButton, &QToolButton::clicked,
            this, &ActionToolbar::compareSnapshots);

    snapshotButton = toolBar->addRibbonAction("Take Snapshot", "take_snapshot",
                                              QIcon(":/icons/snapshot-32.png"),
                                              "memory_profiling");
    connect(snapshotButton, &QToolButton::clicked, this, &ActionToolbar::takeSnapshotAction);

    gcButton = toolBar->addRibbonAction("Start GC",
                                        "start_gc",
                                        QIcon(":/icons/startgc-32.png"),
                                        "memory_profiling");
    connect(gcButton, &QToolButton::clicked, this, &ActionToolbar::performGCAction);


    toolBar->addRibbonAction("Start CPU Profiling",
                             "start_cpu_profile",
                             QIcon(":/icons/disconnect-32.png"),
                             "cpu_profiling");
    toolBar->addRibbonAction("Stop Profiling",
                             "stop_cpu_profiling",
                             QIcon(":/icons/Compare-32.png"),
                             "cpu_profiling");
    toolBar->loadStyleSheet(":/icons/style.css");
}

void ActionToolbar::performGCAction()
{
    emit triggerGc();
}

void ActionToolbar::takeSnapshotAction()
{
    if (snapshotState->snapShotInProgress()) {
        QMessageBox alert(this);
        alert.setText("A snapshot is already in progress");
        alert.exec();
        return;
    } else {
        // set heapsnapshot percentage to 2%
        RBKit::AppState::getInstance()->setAppState("heap_snapshot", 2);
        centralWidget->showStatusMessage("Snapshot Started");
        centralWidget->resetProgressBar();
        centralWidget->setProgressBarValue(2);
        snapshotProgressTimer->start(500);
        snapshotState->setSnapshotProgress(true);
        emit takeSnapshot();
    }
}

void ActionToolbar::attemptConnection()
{

}

void ActionToolbar::compareSnapshots()
{

}

void ActionToolbar::useSelectedHost(QString commandSocket, QString eventSocket)
{
    askHost->close();
    connectionState = CONNECTION_IN_PROGRESS;
    toggleConnectButtonState(connectionState, connectButton);
    emit connectToSocket(commandSocket, eventSocket);
}

void ActionToolbar::connectedToSocket()
{
    RBKit::SqlConnectionPool::getInstance()->setupDatabase();
    enableProfileActions();
    connectionState = CONNECTED;
    toggleConnectButtonState(connectionState, connectButton);
    centralWidget->showStatusMessage("Currently Profiling Ruby application");
    memoryView()->processDetail->displayProcessDetail();
}
