#include "actiontoolbar.h"

#include <QIcon>
#include <QToolButton>


RibbonToolBar *ActionToolbar::getToolBar() const
{
    return toolBar;
}

ActionToolbar::ActionToolbar(CentralWidget *widget)
{
    toolBar = new RibbonToolBar(this);
    connectButton = toolBar->addRibbonAction("Connect", "connect_action", QIcon(":/icons/connect-32.png"));
    connect(connectButton, &QToolButton::clicked, this, &ActionToolbar::attemptConnection);


    toolBar->addRibbonTab("CPU Profiling", "cpu_profiling");
    toolBar->addRibbonTab("Memory Profiling", "memory_profiling");

    gcButton = toolBar->addRibbonAction("Start GC", "start_gc", QIcon(":/icons/startgc-32.png"), "memory_profiling");
    connect(gcButton, &QToolButton::clicked, this, &ActionToolbar::performGCAction);

    snapshotButton = toolBar->addRibbonAction("Take Snapshot", "take_snapshot", QIcon(":/icons/snapshot-32.png"), "memory_profiling");
    connect(snapshotButton, &QToolButton::clicked, this, &ActionToolbar::takeSnapshotAction);

    toolBar->addRibbonAction("Start CPU Profiling", "start_cpu_profile", QIcon(":/icons/disconnect-32.png"), "cpu_profiling");
    toolBar->addRibbonAction("Stop Profiling", "stop_cpu_profiling", QIcon(":/icons/Compare-32.png"), "cpu_profiling");
    toolBar->loadStyleSheet(":/icons/style.css");
}

void ActionToolbar::enableProfileActions()
{
    ui->actionCompare_Heapsnapshots->setEnabled(true);
    ui->actionHeap_Snapshot->setEnabled(true);
    ui->action_Trigger_GC->setEnabled(true);
}

void ActionToolbar::disableProfileActions()
{
    ui->actionCompare_Heapsnapshots->setDisabled(true);
    ui->actionHeap_Snapshot->setDisabled(true);
    ui->action_Trigger_GC->setDisabled(true);
}

void ActionToolbar::performGCAction()
{

}

void ActionToolbar::takeSnapshotAction()
{

}

void ActionToolbar::attemptConnection()
{

}
