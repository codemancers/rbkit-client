#include <QtGui>
#include <QVBoxLayout>
#include <QtNetwork>
#include <QMessageBox>

#include "client.h"

Client::Client(QWidget *parent)
    :   QDialog(parent)
{

    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();


    connectButton = new QPushButton(tr("Connect"));

    connectButton->setDefault(true);
    connectButton->setEnabled(true);
    connectButton->setCheckable(true);
    // false status means currently
    // disconnected
    connectButton->setChecked(false);


    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(connectButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    tcpSocket = new QTcpSocket(this);

    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(toggleButton(bool)));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connectedToSocket()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnectedFromSocket()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("rbkit"));

//    QNetworkConfigurationManager manager;
//    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
//        // Get saved network configuration
//        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
//        settings.beginGroup(QLatin1String("QtNetwork"));
//        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
//        settings.endGroup();

//        // If the saved network configuration is not currently discovered use the system default
//        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
//        if ((config.state() & QNetworkConfiguration::Discovered) !=
//                QNetworkConfiguration::Discovered) {
//            config = manager.defaultConfiguration();
//        }

//        networkSession = new QNetworkSession(config, this);
//        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

//        connectButton->setEnabled(false);
//        statusLabel->setText(tr("Opening network session."));
//        networkSession->open();
//    }
}

void Client::connectToSocket()
{
    connectButton->setEnabled(false);
    connectButton->setText(tr("Connecting.."));
    tcpSocket->abort();
    tcpSocket->connectToHost(ipAddress, PORT);
}

void Client::disconnectFromSocket()
{

    tcpSocket->disconnectFromHost();
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

void Client::readData()
{

    while(tcpSocket->canReadLine())
    {
        QString line = tcpSocket->readLine();
        qDebug() << line;
    }

    connectButton->setEnabled(true);
}

void Client::toggleButton(bool checked)
{

    if( checked ) {
        connectToSocket();
    } else {
        disconnectFromSocket();
    }

}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("rbkit"),
                                 tr("The host was not found."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("rbkit"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "));
        break;
    default:
        QMessageBox::information(this, tr("rbkit"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    disconnectedFromSocket();
}

