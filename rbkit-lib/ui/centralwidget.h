#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "actiontoolbar.h"
#include "common.h"

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class AppMainwindow;

#include "memoryview.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
    QSharedPointer<ActionToolbar> actionToolBar;
    QVBoxLayout* mainLayout;
    QSharedPointer<QTabWidget> chartingTab;
    QSharedPointer<RBKit::MemoryView> memoryView;
    AppMainwindow *mainWindow;

public:
    explicit CentralWidget(AppMainwindow *mainWindow);
    ~CentralWidget();
    void setupCentralView();
    void showStatusMessage(const QString &message) const;
    void setProgressBarValue(int value) const;
    void resetProgressBar() const;

    QSharedPointer<RBKit::MemoryView> getMemoryView() const;
signals:

public slots:
    void onError(const QString &);
};

#endif // CENTRALWIDGET_H
