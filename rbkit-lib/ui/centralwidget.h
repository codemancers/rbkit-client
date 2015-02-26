#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "actiontoolbar.h"

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;

#include "memoryview.h"

class CentralWidget : public QWidget
{
    Q_OBJECT

    ActionToolbar *actionToolBar;
    QVBoxLayout *mainLayout;
    QTabWidget *chartingTab;
    RBKit::MemoryView *memoryView;
public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();
    void setupCentralView();

signals:

public slots:
};

#endif // CENTRALWIDGET_H
