#ifndef APPMAINWINDOW_H
#define APPMAINWINDOW_H

#include <QMainWindow>
#include "centralwidget.h"

class QStatusBar;
class QLabel;
class QProgressBar;

class AppMainwindow : public QMainWindow
{
    Q_OBJECT
    CentralWidget *centralWidget;
    QLabel *statusLabel;

public:
    explicit AppMainwindow(QWidget *parent = 0);
    ~AppMainwindow();
    QStatusBar *appStatusBar;
    QProgressBar *progressBar;
signals:

public slots:
};

#endif // APPMAINWINDOW_H
