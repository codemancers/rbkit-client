#ifndef APPMAINWINDOW_H
#define APPMAINWINDOW_H

#include <QMainWindow>
#include "stackedwidget.h"
#include "centralwidget.h"

class QStatusBar;
class QLabel;
class QProgressBar;
class StackedWidget;

class AppMainwindow : public QMainWindow
{
    Q_OBJECT
    StackedWidget *stackedWidget;
    CentralWidget *centralMemoryWidget;
    CentralWidget *centralCpuWidget;
    QLabel *statusLabel;

public:
    explicit AppMainwindow(QWidget *parent = 0);
    ~AppMainwindow();
    QStatusBar *appStatusBar;
    QProgressBar *progressBar;
    //CpuViewPtr getCpuView();
signals:

public slots:
    void tabChanged(int);
};

#endif // APPMAINWINDOW_H
