#include "centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout();

    toolBar = new RibbonToolBar(this);
    toolBar->addRibbonAction("Connect", "connect_action", QIcon(":/icons/connect-32.png"));
    toolBar->addRibbonTab("CPU Profiling", "cpu_profiling");
    toolBar->addRibbonTab("Memory Profiling", "memory_profiling");

    const QToolButton *btn = toolBar->addRibbonAction("Start GC", "start_gc", QIcon(":/icons/startgc-32.png"), "memory_profiling");
    connect(btn, &QToolButton::clicked, this, &Widget::startGcClicked);
    toolBar->addRibbonAction("Take Snapshot", "take_snapshot", QIcon(":/icons/snapshot-32.png"), "memory_profiling");

    toolBar->addRibbonAction("Start CPU Profiling", "start_cpu_profile", QIcon(":/icons/disconnect-32.png"), "cpu_profiling");
    toolBar->addRibbonAction("Stop Profiling", "stop_cpu_profiling", QIcon(":/icons/Compare-32.png"), "cpu_profiling");
    toolBar->loadStyleSheet(":/icons/style.css");

    mainLayout->addWidget(toolBar, 0, Qt::AlignTop);
    setLayout(mainLayout);
}

CentralWidget::~CentralWidget()
{

}

