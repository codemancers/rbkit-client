#include "rbkitmainwindow.h"
#include "ui_rbkitmainwindow.h"
#include "askhost.h"
#include "jsbridge.h"
#include "appstate.h"
#include "comapresnapshotform.h"

RbkitMainWindow::RbkitMainWindow(QWidget *parent) :
    QMainWindow(parent), connected(false), host(""),
    ui(new Ui::RbkitMainWindow), currentIndex(0)
{
    RBKit::SqlConnectionPool::getInstance()->setupDatabase();
    this->connected = false;
    ui->setupUi(this);

    snapshotProgressTimer = new QTimer(this);

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
    ui->chartingTab->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0,0);
    connect(ui->chartingTab, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClosed(int)));
}


RbkitMainWindow::~RbkitMainWindow()
{
    delete ui;
}

void RbkitMainWindow::addTabWidget(HeapDumpForm *heapDumpForm, const QString& title)
{
    ++currentIndex;
    heapForms[currentIndex] = heapDumpForm;
    heapDumpForm->setParentWindow(this);
    ui->chartingTab->addTab(heapDumpForm, title);
}

void RbkitMainWindow::on_action_Connect_triggered()
{
    qDebug() << "main-window" << connected;

    if (!connected) {
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

QList<int> RbkitMainWindow::diffableSnapshotVersions()
{
   return heapForms.keys();
}


void RbkitMainWindow::useSelectedHost(QString commandsSocket, QString eventsSocket)
{
    askHost->close();
    qDebug() << "Emitting signal with " << eventsSocket;
    emit connectToSocket(commandsSocket, eventsSocket);
}

void RbkitMainWindow::on_action_About_Rbkit_triggered()
{
    QMessageBox msgBox(this);
    msgBox.setText(tr("Rbkit is low overhead profiler for Ruby, brought to you by Codemancers team"));
    msgBox.exec();
}


void RbkitMainWindow::objectDumpAvailable(int snapshotVersion)
{
    HeapDumpForm *heapUI = new HeapDumpForm(this, snapshotVersion);
    heapUI->setParentWindow(this);
    heapUI->loaData();
    ++currentIndex;
    heapForms[currentIndex] = heapUI;
    ui->statusbar->showMessage("Heap snapshot complete");
    ui->statusbar->clearMessage();
    progressBar->setValue(100);
    snapshotProgressTimer->stop();
    ui->chartingTab->addTab(heapUI, QString("Heap Dump #%0").arg(snapshotVersion));
}

void RbkitMainWindow::disconnectFromSocket()
{
    subscriberThread.requestInterruption();
    subscriberThread.exit();
    subscriberThread.wait();
}

void RbkitMainWindow::setupSubscriber()
{
    //Create a subscriber and move it to it's own thread
    subscriber = new Subscriber(memoryView->getJsBridge());
    subscriber->moveToThread(&subscriberThread);

    //Events to/from parent/subcriber thread
    connect(&subscriberThread, &QThread::finished, subscriber, &QObject::deleteLater);
    connect(this, SIGNAL(connectToSocket(QString, QString)),
            subscriber, SLOT(startListening(QString, QString)));
    connect(this, SIGNAL(triggerGc()), subscriber, SLOT(triggerGc()));
    connect(this, SIGNAL(takeSnapshot()), subscriber, SLOT(takeSnapshot()));

    connect(this, SIGNAL(disconnectSubscriber()), subscriber, SLOT(stop()));

    connect(subscriber, &Subscriber::errored, this, &RbkitMainWindow::onError);
    connect(subscriber, &Subscriber::connected, this, &RbkitMainWindow::connectedToSocket);
    connect(subscriber, &Subscriber::disconnected, this, &RbkitMainWindow::disconnectedFromSocket);
    connect(subscriber, &Subscriber::objectDumpAvailable, this, &RbkitMainWindow::objectDumpAvailable);

    subscriberThread.start();
}

void RbkitMainWindow::disconnectedFromSocket()
{
    ui->action_Connect->setText(tr("&Connect"));
    this->connected = false;
    emit disconnectSubscriber();
}


void RbkitMainWindow::connectedToSocket()
{
    ui->action_Connect->setText(tr("&Disconnect"));
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
    RBKit::AppState::getInstance()->setAppState("heap_snapshot", 2);
    ui->statusbar->showMessage("Snapshot started");
    ui->statusbar->clearMessage();
    progressBar->reset();
    progressBar->setValue(2);
    snapshotProgressTimer->start(500);
    emit takeSnapshot();
}

void RbkitMainWindow::tabClosed(int index)
{
    if(index == 0)
        return;

    ui->chartingTab->removeTab(index);
    HeapDumpForm *form = heapForms[index];
    if (form) {
        delete form;
    }

    heapForms.remove(index);
    --currentIndex;
}

void RbkitMainWindow::updateProgressBar()
{
    int currentProgress = RBKit::AppState::getInstance()->getState("heap_snapshot").toInt();
    progressBar->setValue(currentProgress);
}


void RbkitMainWindow::on_actionComapre_Heapsnapshots_triggered()
{
    ComapreSnapshotForm *compareSnapshots = new ComapreSnapshotForm(this);
    QList<int> snapShotVersions = diffableSnapshotVersions();
    if (!snapShotVersions.isEmpty())
        compareSnapshots->setSnapshotVersions(snapShotVersions);
    compareSnapshots->show();
}
