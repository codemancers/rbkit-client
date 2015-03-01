#ifndef RBKITMAINWINDOW_H
#define RBKITMAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QWebFrame>
#include <QMessageBox>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QMap>

#include "subscriber.h"
#include "askhost.h"
#include "heapdumpform.h"
#include "model/objectstore.h"
#include "model/snapshotstate.h"
#include "memoryview.h"

namespace Ui {
class RbkitMainWindow;
}

namespace RBKit {
    class JsBridge;
}

class ActionToolbar;


class RbkitMainWindow : public QMainWindow
{
    Q_OBJECT
    QThread subscriberThread;
    Subscriber *subscriber;
    QString host;
    AskHost *askHost;
    RBKit::MemoryView *memoryView;
    RBKit::SnapshotState *snapShotState;
    QLabel *statusLabel;
    QProgressBar *progressBar;
    QTimer *snapshotProgressTimer;
    bool connected;
    bool connectionInProgress;

    void setupSubscriber();
    void disconnectFromSocket();
    void askForServerInfo();
    void setupToolbarStyle();
    QList<int> diffableSnapshotVersions();
    void disableCloseButtonOnFirstTab();

public:
    explicit RbkitMainWindow(QWidget *parent = 0);
    void addTabWidget(HeapDumpForm* form, const QString &title);
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
    void objectDumpAvailable(int snapshotVersion);
    void useSelectedHost(QString, QString);

    void on_action_Trigger_GC_triggered();

    void on_actionHeap_Snapshot_triggered();
    void tabClosed(int index);
    void updateProgressBar();

    void on_actionCompare_Heapsnapshots_triggered();
    void onDiffSnapshotsSelected(QList<int> selectedSnapshots);
    void receiveYoungGenStats(QVariantMap map);
    void receiveSecondGenStats(QVariantMap map);
    void receiveOldGenStats(QVariantMap map);

private:
    Ui::RbkitMainWindow *ui;
};

#endif // RBKITMAINWINDOW_H
