#include "actiontoolbar.h"

#include <QIcon>
#include <QToolButton>
#include "centralwidget.h"


RibbonToolBar *ActionToolbar::getToolBar() const
{
    return toolBar;
}

ActionToolbar::ActionToolbar(CentralWidget *widget)
    : centralWidget(widget)
{
    Q_INIT_RESOURCE(tool_icons);
    toolBar = new RibbonToolBar(centralWidget);
    connectButton = toolBar->addRibbonAction("Connect", "connect_action", QIcon(":/icons/connect-32.png"));
    connect(connectButton, &QToolButton::clicked, this, &ActionToolbar::attemptConnection);


    toolBar->addRibbonTab("CPU Profiling", "cpu_profiling");
    toolBar->addRibbonTab("Memory Profiling", "memory_profiling");

    compareSnapshotButton = toolBar->addRibbonAction("Compare Snapshot",
                                                     "compare_snapshot",
                                                     QIcon(":/icons/Compare-32.png"),
                                                     "memory_profiling");
    connect(compareSnapshotButton, &QToolButton::clicked, this, &ActionToolbar::compareSnapshots);

    snapshotButton = toolBar->addRibbonAction("Take Snapshot", "take_snapshot", QIcon(":/icons/snapshot-32.png"), "memory_profiling");
    connect(snapshotButton, &QToolButton::clicked, this, &ActionToolbar::takeSnapshotAction);

    gcButton = toolBar->addRibbonAction("Start GC", "start_gc", QIcon(":/icons/startgc-32.png"), "memory_profiling");
    connect(gcButton, &QToolButton::clicked, this, &ActionToolbar::performGCAction);


    toolBar->addRibbonAction("Start CPU Profiling", "start_cpu_profile", QIcon(":/icons/disconnect-32.png"), "cpu_profiling");
    toolBar->addRibbonAction("Stop Profiling", "stop_cpu_profiling", QIcon(":/icons/Compare-32.png"), "cpu_profiling");
    toolBar->loadStyleSheet(":/icons/style.css");
}

void ActionToolbar::enableProfileActions()
{
    gcButton->setEnabled(true);
    compareSnapshotButton->setEnabled(true);
    snapshotButton->setEnabled(true);
    gcButton->setEnabled(true);
}

void ActionToolbar::disableProfileActions()
{
    gcButton->setDisabled(true);
    compareSnapshotButton->setDisabled(true);
    snapshotButton->setDisabled(true);
    gcButton->setDisabled(true);
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

void ActionToolbar::compareSnapshots()
{

}
