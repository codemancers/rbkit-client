#ifndef ACTIONTOOLBAR_H
#define ACTIONTOOLBAR_H


#include <QObject>
#include <QWidget>
#include <QThread>

#include "ribbontoolbar.h"
#include "model/snapshotstate.h"
#include "askhost.h"
#include "subscriber.h"
#include "common.h"
#include "memoryview.h"
#include "appmainwindow.h"
#include "stackedwidget.h"

class QToolButton;
class CentralWidget;
class QTimer;
class AppMainwindow;
class StackedWidget;

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
    AskHost *askHost;
    QString host;
    Subscriber *subscriber;
    ConnectionStates connectionState;
    QThread subscriberThread;
    AppMainwindow &window;
public:
    explicit ActionToolbar(AppMainwindow &window , CentralWidget *widget);
    void enableProfileActions();
    void disableProfileActions();
    void setupToolBar();
    RibbonToolBar *getToolBar() const;
    void setupSubscriber();
    void askForServerInfo();
    RBKit::MemoryView *memoryView() const;
    void disconnectFromSocket();
    void shutDownApp();
    void connectTabChangedSignal(AppMainwindow *window);

private:
    QToolButton *gcButton;
    QToolButton *startCPUButton;
    QToolButton *stopCPUButton;
    QToolButton *connectButton;
    QToolButton *snapshotButton;
    QToolButton *compareSnapshotButton;
signals:
    void connectToSocket(QString commandSocket, QString eventSocket);
    void triggerGc();
    void startCPUProfiling();
    void stopCPUProfiling();
    void disconnectSubscriber();
    void takeSnapshot();
public slots:
    void performGCAction();
    void performStartCPUAction();
    void performStopCPUAction();
    void takeSnapshotAction();
    void attemptConnection();
    void compareSnapshots();
    void useSelectedHost(QString commandSocket, QString eventSocket);
    void connectedToSocket();
    void disconnectedFromSocket();
};

#endif // ACTIONTOOLBAR_H
