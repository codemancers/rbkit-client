#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "actiontoolbar.h"

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;

class CentralWidget : public QWidget
{
    Q_OBJECT

    ActionToolbar *actionToolBar;
    QVBoxLayout *mainLayout;
    QTabWidget *chartingTab;
public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();

signals:

public slots:
};

#endif // CENTRALWIDGET_H
