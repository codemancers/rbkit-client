#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTcpSocket>

class QDialogButtonBox;
class QPushButton;
class QTcpSocket;

class Client : public QDialog
{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);

private slots:
    void connectToSocket();
    void disconnectFromSocket();
    void readData();
    void displayError(QAbstractSocket::SocketError socketError);
    void toggleButton(bool);
    void connectedToSocket();
    void disconnectedFromSocket();

private:
    QPushButton *connectButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QString ipAddress;
    QTcpSocket *tcpSocket;
    static const int PORT = 3333;
};

#endif // CLIENT_H
