#include <QtGui>
#include <QMessageBox>
#include <QWebFrame>

#include "client.h"
#include "ui_dialog.h"

Client::Client(QWidget *parent)
    :   QDialog(parent), ui(new Ui::Dialog)
{
    this->connected = false;
    ui->setupUi(this);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    //Set event for buttons
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(toggleButton()));
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(quitApp()));

    setWindowTitle(tr("rbkit"));

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(onPageLoad(bool)));
    ui->webView->setUrl(QUrl("qrc:/web/web/index.html"));

}

void Client::setupSubscriber()
{
    //Create a subscriber and move it to it's own thread
    subscriber = new Subscriber;
    subscriber->moveToThread(&subscriberThread);

    //Events to/from parent/subcriber thread
    connect(&subscriberThread, &QThread::finished, subscriber, &QObject::deleteLater);
    connect(this, &Client::connectToSocket, subscriber, &Subscriber::startListening);
    connect(subscriber, &Subscriber::messageReady, this, &Client::handleMessage);
    connect(subscriber, &Subscriber::errored, this, &Client::onError);
    connect(subscriber, &Subscriber::connected, this, &Client::connectedToSocket);
    connect(subscriber, &Subscriber::disconnected, this, &Client::disconnectedFromSocket);

    subscriberThread.start();
}

void Client::handleMessage(const QVariantMap& map)
{
   emit sendDatatoJs(map);
}

void Client::connectedToSocket()
{
    ui->connectButton->setEnabled(true);
    ui->connectButton->setText(tr("Disconnect"));
    this->connected = true;
}

void Client::disconnectedFromSocket()
{
    ui->connectButton->setEnabled(true);
    ui->connectButton->setText(tr("Connect"));
    this->connected = false;
}

void Client::toggleButton()
{
    if( !this->connected ) {
        setupSubscriber();
        emit connectToSocket();
    } else {
        disconnectFromSocket();
    }
}

void Client::onError(const QString &error)
{
    disconnectFromSocket();
    QMessageBox::critical(this, tr("rbkit"), error);
}


void Client::disconnectFromSocket()
{
    subscriberThread.requestInterruption();
    subscriberThread.exit();
    subscriberThread.wait();
}

void Client::quitApp()
{
    disconnectFromSocket();
    close();
}

void Client::onPageLoad(bool ok)
{
    qDebug() << ok;

    QWebFrame *frame = ui->webView->page()->mainFrame();
    frame->addToJavaScriptWindowObject(QString("rbkitClient"), this);
}
