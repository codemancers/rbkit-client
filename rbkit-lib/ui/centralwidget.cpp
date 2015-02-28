#include "centralwidget.h"
#include "layoututil.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMessageBox>
#include <QStatusBar>
#include <QProgressBar>

#include "appmainwindow.h"

QSharedPointer<RBKit::MemoryView> CentralWidget::getMemoryView() const
{
    return memoryView;
}

void CentralWidget::onError(const QString &)
{
    QMessageBox::critical(this, tr("rbkit"), error);
}

CentralWidget::CentralWidget(AppMainwindow *window) : QWidget(parent)
  , mainWindow(window)
{
    mainLayout = new QVBoxLayout();
    actionToolBar = QSharedPointer<ActionToolbar>::create(this);
    mainLayout->addWidget(actionToolBar->getToolBar(), 0, Qt::AlignTop);

    chartingTab = QSharedPointer<QTabWidget>::create(this);
    chartingTab->setMinimumHeight(700);
    chartingTab->setMinimumWidth(1100);
    mainLayout->addWidget(chartingTab);

    makeMarginSpacingZero(mainLayout);
    setLayout(mainLayout);
}

CentralWidget::~CentralWidget()
{
    delete mainLayout;
}

void CentralWidget::setupCentralView()
{
    memoryView = QSharedPointer<RBKit::MemoryView>::create();
    chartingTab->addTab(memoryView, "Object Charts");
}

void CentralWidget::showStatusMessage(const QString &message) const
{
    mainWindow->appStatusBar->showMessage(message);
}

void CentralWidget::setProgressBarValue(int value) const
{
    mainWindow->progressBar->setValue(value);
}

void CentralWidget::resetProgressBar() const
{
   mainWindow->progressBar->reset();
}

