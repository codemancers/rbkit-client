#ifndef ACTIONTOOLBAR_H
#define ACTIONTOOLBAR_H

#include "ribbontoolbar.h"

#include <QObject>
#include <QWidget>

class QToolButton;
class CentralWidget;

// Maintains state of Toolbar
class ActionToolbar : public QObject
{
    Q_OBJECT

    RibbonToolBar *toolBar;
    CentralWidget *centralWidget;
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
