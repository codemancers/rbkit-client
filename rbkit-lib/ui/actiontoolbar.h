#ifndef ACTIONTOOLBAR_H
#define ACTIONTOOLBAR_H


#include <QObject>
#include <QWidget>

#include "ribbontoolbar.h"
#include "model/snapshotstate.h"

class QToolButton;
class CentralWidget;
class QTimer;

// Maintains state of Toolbar
class ActionToolbar : public QObject
{
    Q_OBJECT

    RibbonToolBar *toolBar;
    CentralWidget *centralWidget;
    RBKit::SnapshotState *snapshotState;
    QTimer *snapshotProgressTimer;

public:
    explicit ActionToolbar(CentralWidget *widget);
    void enableProfileActions();
    void disableProfileActions();
    RibbonToolBar *getToolBar() const;

private:
    QToolButton *gcButton;
    QToolButton *connectButton;
    QToolButton *snapshotButton;
    QToolButton *compareSnapshotButton;
public slots:
    void performGCAction();
    void takeSnapshotAction();
    void attemptConnection();
    void compareSnapshots();
};

#endif // ACTIONTOOLBAR_H
