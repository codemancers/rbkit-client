#include <QtGui>
#include <QVBoxLayout>
#include <QMessageBox>

#include <msgpack.hpp>

#include "client.h"

Client::Client(QWidget *parent)
    :   QDialog(parent)
{
    context = new zmq::context_t(1);
    socket = new zmq::socket_t(*context, ZMQ_SUB);
    socket->setsockopt(ZMQ_SUBSCRIBE, "", 0);

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

    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(toggleButton(bool)));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("rbkit"));
}

void Client::connectToSocket()
{
    connectButton->setEnabled(false);
    connectButton->setText(tr("Connecting.."));
    try
    {
        socket->connect("tcp://127.0.0.1:5555");
        connectedToSocket();
    }
    catch(zmq::error_t err)
    {
        qDebug() << err.what() ;
        displayError(err.what());
        disconnectFromSocket();
    }
}

void Client::disconnectFromSocket()
{
    socket->close();
    disconnectedFromSocket();
}

void Client::connectedToSocket()
{
    connectButton->setEnabled(true);
    connectButton->setText(tr("Disconnect"));
    connectButton->setChecked(true);
    readData();
}

void Client::disconnectedFromSocket()
{
    connectButton->setEnabled(true);
    connectButton->setText(tr("Connect"));
    connectButton->setChecked(false);
}

void Client::readData()
{

    while(true)
    {
        zmq::message_t message;
        if(socket->recv(&message, ZMQ_NOBLOCK))
        {
            QString x = QString::fromUtf8((const char *)message.data(), message.size());
            qDebug() << x;
        }
    }
}

void Client::toggleButton(bool checked)
{

    if( checked ) {
        connectToSocket();
    } else {
        disconnectFromSocket();
    }

}

void Client::displayError(const char * error)
{
    QMessageBox::information(this, tr("rbkit"), error);
    disconnectedFromSocket();
}

