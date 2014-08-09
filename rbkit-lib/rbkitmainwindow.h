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
    void sendGcStatsToJs(const QVariantMap& map);
    void sendDatatoJs(const QVariantMap& map);

    void connectToSocket(QString, QString);
    void triggerGc();
    void disconnectSubscriber();

private slots:
    void on_action_Connect_triggered();

    void on_action_Quit_triggered();

    void on_action_About_Rbkit_triggered();
    void handleMessage(const QVariantMap& map);
    void handleGcStats(const QVariantMap& map);
    void connectedToSocket();
    void disconnectedFromSocket();
    void onError(const QString &);
    void onPageLoad(bool ok);
    void useSelectedHost(QString, QString);

    void on_action_Trigger_GC_triggered();

private:
    Ui::RbkitMainWindow *ui;
};

#endif // RBKITMAINWINDOW_H
