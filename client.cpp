#include <QtGui>
#include <QVBoxLayout>
#include <QMessageBox>

#include "client.h"

Client::Client(QWidget *parent)
    :   QDialog(parent)
{
    connectButton = new QPushButton(tr("Connect"));
    connectButton->setDefault(true);
    connectButton->setEnabled(true);
    connectButton->setCheckable(true);
    // false status means currently disconnected
    connectButton->setChecked(false);


    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(connectButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    //Set event for buttons
    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(toggleButton(bool)));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quitApp()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("rbkit"));
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
