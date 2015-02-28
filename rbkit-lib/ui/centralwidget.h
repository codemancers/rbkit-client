#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "actiontoolbar.h"
#include "common.h"

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;

#include "memoryview.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
    QSharedPointer<ActionToolbar> actionToolBar;
    QVBoxLayout* mainLayout;
    QSharedPointer<QTabWidget> chartingTab;
    QSharedPointer<RBKit::MemoryView> memoryView;

public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();
    void setupCentralView();

    QSharedPointer<RBKit::MemoryView> getMemoryView() const;
signals:

public slots:
};

#endif // CENTRALWIDGET_H
