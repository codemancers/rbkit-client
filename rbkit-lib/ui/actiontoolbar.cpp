#include "actiontoolbar.h"

#include <QIcon>
#include <QToolButton>
#include <QStatusBar>
#include <QMessageBox>
#include <QVariantMap>

#include "centralwidget.h"
#include "processdetail.h"

static void toggleConnectButtonState(ConnectionStates state, QToolButton *btn) {
    switch (state) {
    case CONNECTED:
        btn->setText("&Disconnect");
        btn->setIcon(QIcon(":/icons/disconnect-32.png"));
        break;
    case DISCONNECTED:
        btn->setText("&Connect");
        btn->setIcon(QIcon(":/icons/connect-32.png"));
        break;
    default:
        btn->setText("&Disconnect");
        btn->setIcon(QIcon(":/icons/disconnect-32.png"));
        break;
    }
}

ActionToolbar::ActionToolbar(AppMainwindow *window, CentralWidget *widget)
    : centralWidget(widget), window(window)
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
    connect(this, SIGNAL(startCPUProfiling()), subscriber, SLOT(startCPUProfiling()));
    connect(this, SIGNAL(stopCPUProfiling()), subscriber, SLOT(stopCPUProfiling()));

    connect(subscriber, &Subscriber::errored, centralWidget, &CentralWidget::onError);
    connect(subscriber, &Subscriber::connected, this, &ActionToolbar::connectedToSocket);
    connect(subscriber, &Subscriber::disconnected, this,
            &ActionToolbar::disconnectedFromSocket);
    connect(subscriber, &Subscriber::objectDumpAvailable,
            centralWidget, &CentralWidget::objectDumpAvailable);

    connect(subscriber, &Subscriber::youngGenStats,
            centralWidget, &CentralWidget::receiveYoungGenStats);
    connect(subscriber, &Subscriber::secondGenStats,
            centralWidget, &CentralWidget::receiveSecondGenStats);
    connect(subscriber, &Subscriber::oldGenStats,
            centralWidget, &CentralWidget::receiveOldGenStats);

    subscriberThread.start();
}

void ActionToolbar::askForServerInfo()
{
    if(host.size() == 0) {
        askHost = new AskHost();
        connect(askHost, SIGNAL(userHasSelectedHost(QString, QString)),
                this, SLOT(useSelectedHost(QString, QString)));
        askHost->show();
    }
}

RBKit::MemoryView *ActionToolbar::memoryView() const
{
    return centralWidget->getMemoryView().data();
}

void ActionToolbar::disconnectFromSocket()
{
    qDebug() << "Attempting to stop the thread";
    subscriberThread.requestInterruption();
    subscriberThread.exit();
    subscriberThread.wait();
    qDebug() << "Thread has been stopped";
}

void ActionToolbar::shutDownApp()
{
    disconnectFromSocket();
}

void ActionToolbar::setupToolBar()
{
    toolBar = new RibbonToolBar(centralWidget);
    connectButton = toolBar->addRibbonAction("Connect", "connect_action", QIcon(":/icons/connect-32.png"));
    connect(connectButton, &QToolButton::clicked, this, &ActionToolbar::attemptConnection);


    toolBar->addRibbonTab("Memory Profiling", "memory_profiling");
    toolBar->addRibbonTab("CPU Profiling", "cpu_profiling");

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


    startCPUButton = toolBar->addRibbonAction("Start CPU Profiling",
                             "start_cpu_profile",
                             QIcon(":/icons/disconnect-32.png"),
                             "cpu_profiling");
    connect(startCPUButton, &QToolButton::clicked, this, &ActionToolbar::performStartCPUAction);

    stopCPUButton = toolBar->addRibbonAction("Stop Profiling",
                             "stop_cpu_profiling",
                             QIcon(":/icons/Compare-32.png"),
                             "cpu_profiling");
    connect(stopCPUButton, &QToolButton::clicked, this, &ActionToolbar::performStopCPUAction);
    connect(this, SIGNAL(stopCPUProfiling()), window, SLOT(newCpuView()));

    toolBar->loadStyleSheet(":/icons/style.css");
}

void ActionToolbar::performGCAction()
{
    emit triggerGc();
}

void ActionToolbar::performStartCPUAction() {
    emit startCPUProfiling();
}

void ActionToolbar::performStopCPUAction() {
    emit stopCPUProfiling();
}

void ActionToolbar::takeSnapshotAction()
{
    if (centralWidget->attemptMemorySnapshot()) {
        emit takeSnapshot();
    }
}

void ActionToolbar::attemptConnection()
{
    if (connectionState == DISCONNECTED) {
        setupSubscriber();
        askForServerInfo();
    } else {
        disconnectFromSocket();
    }
}

void ActionToolbar::compareSnapshots()
{
    centralWidget->compareSnapshots();
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

void ActionToolbar::disconnectedFromSocket()
{
    connectionState = DISCONNECTED;
    toggleConnectButtonState(connectionState, connectButton);
    centralWidget->appDisconnected();
    disableProfileActions();
}

void ActionToolbar::connectTabChangedSignal(AppMainwindow *window)
{
    connect(toolBar,
            SIGNAL(changeTab(int)),
            window,
            SLOT(tabChanged(int)));
}
