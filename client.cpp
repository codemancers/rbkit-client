#include <QtGui>
#include <QMessageBox>
#include <QWebFrame>

#include "client.h"
#include "ui_dialog.h"

Client::Client(QWidget *parent)
    :   QDialog(parent), ui(new Ui::Dialog)
{
    connectButton = new QPushButton(tr("Connect"));
    connectButton->setDefault(true);
    connectButton->setEnabled(true);
    connectButton->setCheckable(true);
    // false status means currently disconnected
    connectButton->setChecked(false);


    quitButton = new QPushButton(tr("Quit"));

    ui->setupUi(this);


    ui->buttonBox->addButton(connectButton, QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    //Set event for buttons
    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(toggleButton(bool)));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quitApp()));

    setWindowTitle(tr("rbkit"));

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(onPageLoad(bool)));
    ui->webView->setUrl(QUrl("qrc:/web/graph.html"));
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

void Client::handleMessage(const QString &msg)
{
   qDebug() << msg;
}

void Client::connectedToSocket()
{
    connectButton->setEnabled(true);
    connectButton->setText(tr("Disconnect"));
    connectButton->setChecked(true);
}

void Client::disconnectedFromSocket()
{
    connectButton->setEnabled(true);
    connectButton->setText(tr("Connect"));
    connectButton->setChecked(false);
}

void Client::toggleButton(bool checked)
{

    if( checked ) {
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

    // get the contents for graphjs, and evaluate it.
    QFile file(":/web/graph.js");
    file.open(QIODevice::ReadOnly);
    QString graphjs = file.readAll();
    file.close();

    frame->evaluateJavaScript(graphjs);
}
