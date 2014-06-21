#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QPushButton>
#include <QThread>

#include "subscriber.h"

namespace Ui {
class Dialog;
}

class QPushButton;

class Client : public QDialog
{
    Q_OBJECT

    QThread subscriberThread;
    Subscriber *subscriber;
    Ui::Dialog *ui;

    void setupSubscriber();
    void disconnectFromSocket();

public:
    Client(QWidget *parent = 0);

signals:
    void sendDatatoJs(const QVariantMap& map);
private:
    bool connected;

private slots:
    void toggleButton();
    void handleMessage(const QVariantMap& map);
    void connectedToSocket();
    void disconnectedFromSocket();
    void onError(const QString &);
    void quitApp();
    void onPageLoad(bool ok);

signals:
    void connectToSocket();

};

#endif // CLIENT_H
