#include "actiontoolbar.h"

#include <QIcon>
#include <QToolButton>
#include "centralwidget.h"

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
    subscriber = new Subscriber(memoryView->getJsBridge());
    subscriber->moveToThread(&subscriberThread);

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

}

void ActionToolbar::takeSnapshotAction()
{

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
    connectButton->setText(tr("&Disconnect"));
    connectButton->setIcon(QIcon(":/icons/disconnect-32.png"));
    emit connectToSocket(commandSocket, eventSocket);
}
