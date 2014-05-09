#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QThread>

#include "subscriber.h"

class QDialogButtonBox;
class QPushButton;

class Client : public QDialog
{
    Q_OBJECT

    QPushButton *connectButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QThread subscriberThread;
    Subscriber *subscriber;

    void setupSubscriber();

public:
    Client(QWidget *parent = 0);

private slots:
    void toggleButton(bool);
    void handleMessage(const QString &);
    void connectedToSocket();
    void disconnectedFromSocket();
    void disconnectFromSocket();
    void onError(const QString &);
    void quitApp();

signals:
    void connectToSocket();

};

#endif // CLIENT_H
