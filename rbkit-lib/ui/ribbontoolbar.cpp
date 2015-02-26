#include "ribbontoolbar.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabBar>
#include <QToolButton>
#include <QIcon>
#include <QSpacerItem>
#include <QFile>
#include <QDebug>
#include "layoututil.hpp"

void RibbonToolBar::insertRibbonWidget()
{
    ribbonUiWidget = new QWidget(this);
    ribbonUiWidget->setMinimumWidth(600);
    ribbonUiWidget->setObjectName("ribbon_tab_container");
    ribbonUiWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout0->addWidget(ribbonUiWidget, 0, 0);

    layout1 = new QVBoxLayout();
    tabBar = new QTabBar(this);
    // give a name to the TabBar to be used from CSS
    tabBar->setObjectName("action_tabbar");
    tabBar->setMaximumHeight(20);
    tabBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    connect(tabBar, &QTabBar::currentChanged, this, &RibbonToolBar::tabSelectionChanged);
    layout1->addWidget(tabBar, 0, Qt::AlignTop);

    makeMarginSpacingZero(layout1);
    ribbonUiWidget->setLayout(layout1);
}

RibbonToolBar::RibbonToolBar(QWidget *parent) : QWidget(parent), tabIndexes(0)
{
    // give a name to the entire widget to be used from css
    setObjectName("ribbon_toolbar");
    setMaximumHeight(48);

    layout0 = new QHBoxLayout(this);
    insertRibbonWidget();
    makeMarginSpacingZero(layout0);
    setLayout(layout0);
}

RibbonToolBar::~RibbonToolBar()
{

}

void RibbonToolBar::loadStyleSheet(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());

    qApp->setStyleSheet(styleSheet);
}


// this adds a action to main toolbar
QToolButton *RibbonToolBar::addRibbonAction(const QString &actionName,
                                    const QString &actionIdentifier,
                                    const QIcon &icon)
{
    QToolButton *actionButton = new QToolButton(this);
    actionButton->setObjectName(actionIdentifier);
    actionButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    actionButton->setIcon(icon);
    actionButton->setText(actionName);
    actionButton->setMinimumHeight(48);
    toolBarButtons[actionIdentifier] = actionButton;
    layout0->insertWidget(0, actionButton, 0, Qt::AlignLeft | Qt::AlignTop);
    return actionButton;
}

void RibbonToolBar::addRibbonTab(const QString &tabName,
                                 const QString &tabIdentifier)
{
    tabBar->addTab(tabName);
    QWidget *actionHolder = new QWidget(this);
    actionHolder->setObjectName(tabIdentifier);
    QHBoxLayout *actionLayout = new QHBoxLayout();
    QSpacerItem *spacer = new QSpacerItem(100, 32, QSizePolicy::MinimumExpanding);
    actionLayout->addSpacerItem(spacer);
    if (tabIndexes == 0) {
        qDebug() << "Adding " << tabName << " to a visible tab";
        containerWidgets[tabIdentifier] = new TabContainerWidget(actionHolder, tabIndexes, true, tabIdentifier, actionLayout);
        layout1->addWidget(actionHolder);
    } else {
        qDebug() << "Adding " << tabName << " to a hidden tab";
        actionHolder->hide();
        containerWidgets[tabIdentifier] = new TabContainerWidget(actionHolder, tabIndexes, false, tabIdentifier, actionLayout);
    }
    makeMarginSpacingZero(actionLayout);
    actionHolder->setLayout(actionLayout);

    tabIndexes++;
}

// this adds an action to one of the tabs
QToolButton *RibbonToolBar::addRibbonAction(const QString &actionName,
                                    const QString &actionIdentifier,
                                    const QIcon &icon,
                                    const QString &tabIdentifier)
{
    TabContainerWidget *containerData = containerWidgets[tabIdentifier];
    QToolButton *actionButton = new QToolButton(this);
    actionButton->setObjectName(actionIdentifier);
    actionButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    actionButton->setIcon(icon);
    actionButton->setText(actionName);
    containerData->containerLayout->insertWidget(0, actionButton);
    toolBarButtons[actionIdentifier] = actionButton;
    return actionButton;
}

void RibbonToolBar::tabSelectionChanged(int index)
{
    for(TabContainerWidget *containerData : containerWidgets) {
        if (containerData->index == index) {
            containerData->showWidget(layout1);
        } else {
            containerData->hideWidget(layout1);
        }
    }
}

