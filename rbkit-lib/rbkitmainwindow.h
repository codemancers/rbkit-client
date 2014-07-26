#ifndef RBKITMAINWINDOW_H
#define RBKITMAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QWebFrame>
#include <QMessageBox>

#include "subscriber.h"
#include "askhost.h"

namespace Ui {
class RbkitMainWindow;
}

class RbkitMainWindow : public QMainWindow
{
    Q_OBJECT
    QThread subscriberThread;
    Subscriber *subscriber;
    void setupSubscriber();
    void disconnectFromSocket();
    void askForServerInfo();
    bool connected;
    QString host;
    AskHost *askHost;

public:
    explicit RbkitMainWindow(QWidget *parent = 0);
    ~RbkitMainWindow();

signals:
    void sendDatatoJs(const QVariantMap& map);
    void connectToSocket(const QString&);

private slots:
    void on_action_Connect_triggered();

    void on_action_Quit_triggered();

    void on_action_About_Rbkit_triggered();
    void handleMessage(const QVariantMap& map);
    void connectedToSocket();
    void disconnectedFromSocket();
    void onError(const QString &);
    void onPageLoad(bool ok);
    void useSelectedHost(const QString& selectedHost);

private:
    Ui::RbkitMainWindow *ui;
};

#endif // RBKITMAINWINDOW_H
