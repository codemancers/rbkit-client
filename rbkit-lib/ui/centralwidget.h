#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "ribbontoolbar.h"

class QVBoxLayout;
class QHBoxLayout;

class CentralWidget : public QWidget
{
    Q_OBJECT
    RibbonToolBar *toolBar;
    QVBoxLayout *mainLayout;
public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();

signals:

public slots:
};

#endif // CENTRALWIDGET_H
