#ifndef APPMAINWINDOW_H
#define APPMAINWINDOW_H

#include <QMainWindow>
#include "centralwidget.h"

class QStatusBar;

class AppMainwindow : public QMainWindow
{
    Q_OBJECT
    CentralWidget *centralWidget;
    QStatusBar *appStatusBar;
public:
    explicit AppMainwindow(QWidget *parent = 0);
    ~AppMainwindow();

signals:

public slots:
};

#endif // APPMAINWINDOW_H
