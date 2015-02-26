#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "actiontoolbar.h"
#include "ribbontoolbar.h"

class QVBoxLayout;
class QHBoxLayout;

class CentralWidget : public QWidget
{
    Q_OBJECT
    ActionToolbar *actionToolBar;
    QVBoxLayout *mainLayout;
public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();

signals:

public slots:
};

#endif // CENTRALWIDGET_H
