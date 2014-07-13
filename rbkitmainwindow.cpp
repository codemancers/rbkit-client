#include "rbkitmainwindow.h"
#include "ui_rbkitmainwindow.h"
#include "askhost.h"

RbkitMainWindow::RbkitMainWindow(QWidget *parent) :
    QMainWindow(parent), connected(false), host(""),
    ui(new Ui::RbkitMainWindow)
{
    this->connected = false;
    ui->setupUi(this);
    QWebSettings *settings = ui->chartingView->settings();
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::AutoLoadImages, true);
    settings->setMaximumPagesInCache(0);
    settings->setObjectCacheCapacities(0, 0, 0);
    settings->setIconDatabasePath("");

    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::PluginsEnabled, false);
    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::PrivateBrowsingEnabled, false);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);

    connect(ui->chartingView, SIGNAL(loadFinished(bool)), this, SLOT(onPageLoad(bool)));
    ui->chartingView->setUrl(QUrl("qrc:/web/web/index.html"));
}

RbkitMainWindow::~RbkitMainWindow()
{
    delete ui;
}


void RbkitMainWindow::on_action_Connect_triggered()
{
    if( !this->connected ) {
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
        connect(this->askHost, SIGNAL(userHasSelectedHost(QString)), this, SLOT(useSelectedHost(const QString &)));
        askHost->show();
    }
}

void RbkitMainWindow::useSelectedHost(const QString &selectedHost) {
    host = selectedHost;
    askHost->close();
    qDebug() << "Emitting signal with " << host;
    emit connectToSocket(host);
}

void RbkitMainWindow::on_action_About_Rbkit_triggered()
{
    QMessageBox msgBox(this);
    msgBox.setText(tr("Rbkit is low overhead profiler for Ruby, brought to you by Codemancers team"));
    msgBox.exec();
}

void RbkitMainWindow::onPageLoad(bool ok)
{
    QWebFrame *frame = ui->chartingView->page()->mainFrame();
    frame->addToJavaScriptWindowObject(QString("rbkitClient"), this);
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
    subscriber = new Subscriber;
    subscriber->moveToThread(&subscriberThread);

    //Events to/from parent/subcriber thread
    connect(&subscriberThread, &QThread::finished, subscriber, &QObject::deleteLater);
    connect(this, SIGNAL(connectToSocket(const QString&)), subscriber, SLOT(startListening(const QString&)));

    connect(subscriber, &Subscriber::messageReady, this, &RbkitMainWindow::handleMessage);
    connect(subscriber, &Subscriber::errored, this, &RbkitMainWindow::onError);
    connect(subscriber, &Subscriber::connected, this, &RbkitMainWindow::connectedToSocket);
    connect(subscriber, &Subscriber::disconnected, this, &RbkitMainWindow::disconnectedFromSocket);

    subscriberThread.start();
}

void RbkitMainWindow::disconnectedFromSocket()
{
    ui->action_Connect->setText(tr("&Connect"));
    this->connected = false;
}

void RbkitMainWindow::handleMessage(const QVariantMap& map)
{
    emit sendDatatoJs(map);
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
