#ifndef ACTIONTOOLBAR_H
#define ACTIONTOOLBAR_H


#include <QObject>
#include <QWidget>

#include "ribbontoolbar.h"
#include "model/snapshotstate.h"
#include "askhost.h"
#include "subscriber.h"

class QToolButton;
class CentralWidget;
class QTimer;

enum ConnectionStates {
    DISCONNECTED = 0,
    CONNECTION_IN_PROGRESS,
    CONNECTED
};

// Maintains state of Toolbar
class ActionToolbar : public QObject
{
    Q_OBJECT

    RibbonToolBar *toolBar;
    CentralWidget *centralWidget;
    RBKit::SnapshotState *snapshotState;
    QTimer *snapshotProgressTimer;
    AskHost *askHost;
    QString *host;
    Subscriber *subscriber;
    ConnectionStates connectionState;
public:
    explicit ActionToolbar(CentralWidget *widget);
    void enableProfileActions();
    void disableProfileActions();
    void setupToolBar();
    RibbonToolBar *getToolBar() const;
    void setupSubscriber();
    void askForServerInfo();

private:
    QToolButton *gcButton;
    QToolButton *connectButton;
    QToolButton *snapshotButton;
    QToolButton *compareSnapshotButton;
signals:
    void connectToSocket(QString commandSocket, QString eventSocket);
    void triggerGc();
    void disconnectSubscriber();
    void takeSnapshot();
public slots:
    void performGCAction();
    void takeSnapshotAction();
    void attemptConnection();
    void compareSnapshots();
    void useSelectedHost(QString commandSocket, QString eventSocket);
};

#endif // ACTIONTOOLBAR_H
