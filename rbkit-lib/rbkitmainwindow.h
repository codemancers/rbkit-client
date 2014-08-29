#ifndef RBKITMAINWINDOW_H
#define RBKITMAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QWebFrame>
#include <QMessageBox>

#include "subscriber.h"
#include "askhost.h"
#include "heapdumpform.h"
#include "objectstore.h"
#include "memoryview.h"

namespace Ui {
class RbkitMainWindow;
}


namespace RBKit {
    class JsBridge;
}


class RbkitMainWindow : public QMainWindow
{
    Q_OBJECT
    QThread subscriberThread;
    Subscriber *subscriber;
    QString host;
    AskHost *askHost;
    RBKit::MemoryView *memoryView;

    void setupSubscriber();
    void disconnectFromSocket();
    void askForServerInfo();
    bool connected;

public:
    explicit RbkitMainWindow(QWidget *parent = 0);
    ~RbkitMainWindow();

signals:
    void connectToSocket(QString, QString);
    void triggerGc();
    void disconnectSubscriber();
    void takeSnapshot();

private slots:
    void on_action_Connect_triggered();

    void on_action_Quit_triggered();

    void on_action_About_Rbkit_triggered();
    void connectedToSocket();
    void disconnectedFromSocket();
    void onError(const QString &);
    void objectDumpAvailable(const RBKit::ObjectStore&);
    void useSelectedHost(QString, QString);

    void on_action_Trigger_GC_triggered();

    void on_actionHeap_Snapshot_triggered();

private:
    Ui::RbkitMainWindow *ui;
};

#endif // RBKITMAINWINDOW_H
