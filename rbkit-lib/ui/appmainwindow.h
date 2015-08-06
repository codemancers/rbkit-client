#ifndef APPMAINWINDOW_H
#define APPMAINWINDOW_H

#include <QMainWindow>
#include "stackedwidget.h"
#include "centralwidget.h"
#include "actiontoolbar.h"
#include "cpuview.h"

class QStatusBar;
class QLabel;
class QProgressBar;
class StackedWidget;
class ActionToolbar;

class AppMainwindow : public QMainWindow
{
    Q_OBJECT
    StackedWidget *stackedWidget;
    CentralWidget *centralMemoryWidget;
    QTabWidget *cpuTab;
    QLabel *statusLabel;

    QSharedPointer<ActionToolbar> actionToolBar;
    QVBoxLayout *mainLayout;

    QList<QSharedPointer<CpuView>> cpuViewList;

public:
    explicit AppMainwindow(QWidget *parent = 0);
    ~AppMainwindow();
    QStatusBar *appStatusBar;
    QProgressBar *progressBar;
    //CpuViewPtr getCpuView();
signals:

public slots:
    void tabChanged(int);
    void newCpuView();
    void closeCpuTab(int);
};

#endif // APPMAINWINDOW_H
