#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "actiontoolbar.h"
#include "common.h"

#include <QVariantMap>
#include <QTimer>
#include <QList>

class QVBoxLayout;
class QHBoxLayout;
class QTabWidget;
class AppMainwindow;
class HeapDumpForm;
class ActionToolbar;

#include "memoryview.h"
#include "model/snapshotstate.h"
#include "cpuview.h"
#include <QStackedWidget>


class CentralWidget : public QWidget
{
    Q_OBJECT
    QSharedPointer<ActionToolbar> actionToolBar;
    QVBoxLayout* mainLayout;
    QSharedPointer<QTabWidget> chartingTab;
    QSharedPointer<RBKit::MemoryView> memoryView;
    AppMainwindow *mainWindow;
    RBKit::SnapshotState *snapshotState;
    QTimer *snapshotProgressTimer;
    QList<int> diffableSnapshotVersions();

public:
    explicit CentralWidget(StackedWidget *mainWindow, AppMainwindow *window);
    ~CentralWidget();
    void setupCentralView();
    void showStatusMessage(const QString &message) const;
    void setProgressBarValue(int value) const;
    void resetProgressBar() const;
    bool attemptMemorySnapshot();

    QSharedPointer<RBKit::MemoryView> getMemoryView() const;
    void compareSnapshots();
    void addTabWidget(HeapDumpForm* form, const QString &title);
    void appDisconnected();
signals:

public slots:
    void onError(const QString &error);
    void receiveYoungGenStats(QVariantMap map);
    void receiveSecondGenStats(QVariantMap map);
    void receiveOldGenStats(QVariantMap map);
    void objectDumpAvailable(int snapshotVersion);
    void updateProgressBar();
private slots:
    void tabClosed(int index);
    void onDiffSnapshotsSelected(QList<int> selectedSnapshots);
};

#endif // CENTRALWIDGET_H
