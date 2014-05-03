#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>

#include <zmq.hpp>

class QDialogButtonBox;
class QPushButton;

class Client : public QDialog
{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);

private slots:
    void toggleButton(bool);

private:
    QPushButton *connectButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    zmq::socket_t *socket;
    zmq::context_t *context;

    void connectToSocket();
    void disconnectFromSocket();
    void readData();
    void displayError(const char *error);
    void connectedToSocket();
    void disconnectedFromSocket();
};

#endif // CLIENT_H
