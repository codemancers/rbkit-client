/********************************************************************************
** Form generated from reading UI file 'rbkitmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RBKITMAINWINDOW_H
#define UI_RBKITMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RbkitMainWindow
{
public:
    QAction *action_Connect;
    QAction *action_Quit;
    QAction *action_About_Rbkit;
    QAction *action_Trigger_GC;
    QAction *actionHeap_Snapshot;
    QAction *actionComapre_Heapsnapshots;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *chartingTab;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Help;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *RbkitMainWindow)
    {
        if (RbkitMainWindow->objectName().isEmpty())
            RbkitMainWindow->setObjectName(QStringLiteral("RbkitMainWindow"));
        RbkitMainWindow->resize(1100, 840);
        RbkitMainWindow->setMinimumSize(QSize(850, 840));
        action_Connect = new QAction(RbkitMainWindow);
        action_Connect->setObjectName(QStringLiteral("action_Connect"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/connect-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Connect->setIcon(icon);
        action_Quit = new QAction(RbkitMainWindow);
        action_Quit->setObjectName(QStringLiteral("action_Quit"));
        action_About_Rbkit = new QAction(RbkitMainWindow);
        action_About_Rbkit->setObjectName(QStringLiteral("action_About_Rbkit"));
        action_Trigger_GC = new QAction(RbkitMainWindow);
        action_Trigger_GC->setObjectName(QStringLiteral("action_Trigger_GC"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/startgc-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Trigger_GC->setIcon(icon1);
        actionHeap_Snapshot = new QAction(RbkitMainWindow);
        actionHeap_Snapshot->setObjectName(QStringLiteral("actionHeap_Snapshot"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/snapshot-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHeap_Snapshot->setIcon(icon2);
        actionComapre_Heapsnapshots = new QAction(RbkitMainWindow);
        actionComapre_Heapsnapshots->setObjectName(QStringLiteral("actionComapre_Heapsnapshots"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/Comapre-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionComapre_Heapsnapshots->setIcon(icon3);
        centralwidget = new QWidget(RbkitMainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        chartingTab = new QTabWidget(centralwidget);
        chartingTab->setObjectName(QStringLiteral("chartingTab"));
        chartingTab->setTabPosition(QTabWidget::North);
        chartingTab->setTabsClosable(true);

        horizontalLayout->addWidget(chartingTab);

        RbkitMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RbkitMainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1100, 24));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menu_Help = new QMenu(menubar);
        menu_Help->setObjectName(QStringLiteral("menu_Help"));
        RbkitMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RbkitMainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        RbkitMainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(RbkitMainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setAutoFillBackground(true);
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(16, 16));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->setFloatable(false);
        RbkitMainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_Help->menuAction());
        menu_File->addAction(action_Connect);
        menu_File->addAction(action_Trigger_GC);
        menu_File->addAction(action_Quit);
        menu_Help->addAction(action_About_Rbkit);
        toolBar->addSeparator();
        toolBar->addAction(action_Connect);
        toolBar->addAction(action_Trigger_GC);
        toolBar->addAction(actionHeap_Snapshot);
        toolBar->addAction(actionComapre_Heapsnapshots);

        retranslateUi(RbkitMainWindow);

        chartingTab->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(RbkitMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RbkitMainWindow)
    {
        RbkitMainWindow->setWindowTitle(QApplication::translate("RbkitMainWindow", "Rbkit - A Ruby Profiler", 0));
        action_Connect->setText(QApplication::translate("RbkitMainWindow", "&Connect", 0));
        action_Quit->setText(QApplication::translate("RbkitMainWindow", "&Quit", 0));
        action_About_Rbkit->setText(QApplication::translate("RbkitMainWindow", "&About Rbkit", 0));
        action_Trigger_GC->setText(QApplication::translate("RbkitMainWindow", "&Start GC", 0));
        actionHeap_Snapshot->setText(QApplication::translate("RbkitMainWindow", "Heap Snapshot", 0));
#ifndef QT_NO_TOOLTIP
        actionHeap_Snapshot->setToolTip(QApplication::translate("RbkitMainWindow", "Record heapsnapshot", 0));
#endif // QT_NO_TOOLTIP
        actionComapre_Heapsnapshots->setText(QApplication::translate("RbkitMainWindow", "Comapre  Snapshots", 0));
#ifndef QT_NO_TOOLTIP
        actionComapre_Heapsnapshots->setToolTip(QApplication::translate("RbkitMainWindow", "Comapre Heapsnpshot", 0));
#endif // QT_NO_TOOLTIP
        menu_File->setTitle(QApplication::translate("RbkitMainWindow", "&File", 0));
        menu_Help->setTitle(QApplication::translate("RbkitMainWindow", "&Help", 0));
        toolBar->setWindowTitle(QApplication::translate("RbkitMainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class RbkitMainWindow: public Ui_RbkitMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RBKITMAINWINDOW_H
