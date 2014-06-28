#include "rbkitmainwindow.h"
#include "ui_rbkitmainwindow.h"

RbkitMainWindow::RbkitMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RbkitMainWindow)
{
    this->connected = false;
    ui->setupUi(this);
    connect(ui->chartingView, SIGNAL(loadFinished(bool)), this, SLOT(onPageLoad(bool)));
    ui->chartingView->setUrl(QUrl("qrc:/web/graph.html"));
}

RbkitMainWindow::~RbkitMainWindow()
{
    delete ui;
}

void RbkitMainWindow::on_action_Connect_triggered()
{
    if( !this->connected ) {
        setupSubscriber();
        emit connectToSocket();
    } else {
        disconnectFromSocket();
    }
}

void RbkitMainWindow::on_action_Quit_triggered()
{
    disconnectFromSocket();
    close();
}

void RbkitMainWindow::on_action_About_Rbkit_triggered()
{
    QMessageBox msgBox;
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
    connect(this, &RbkitMainWindow::connectToSocket, subscriber, &Subscriber::startListening);
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
