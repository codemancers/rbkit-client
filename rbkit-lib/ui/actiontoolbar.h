#ifndef ACTIONTOOLBAR_H
#define ACTIONTOOLBAR_H

#include "ribbontoolbar.h"
#include "centralwidget.h"

#include <QObject>

clas QToolButton;

// Maintains state of Toolbar
class ActionToolbar : public QObject
{
    Q_OBJECT

    RibbonToolBar *toolBar;
public:
    explicit ActionToolbar(CentralWidget *widget);
    void enableProfileActions();
    void disableProfileActions();
    RibbonToolBar *getToolBar() const;

private:
    QToolButton *gcButton;
    QToolButton *connectButton;
    QToolButton *snapshotButton;
public slots:
    void performGCAction();
    void takeSnapshotAction();
    void attemptConnection();
};

#endif // ACTIONTOOLBAR_H
