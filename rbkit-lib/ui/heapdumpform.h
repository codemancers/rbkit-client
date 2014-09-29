#ifndef HEAPDUMPFORM_H
#define HEAPDUMPFORM_H

#include <QWidget>
#include <QTableView>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QContextMenuEvent>
#include <QPoint>
#include <QAction>
#include <QMenu>
#include <QSharedPointer>

#include "model/objectstore.h"
#include "model/heapdatamodel.h"
#include "model/heapitem.h"
#include "sqlconnectionpool.h"
#include "model/sortobjectproxymodel.h"

namespace Ui {
class HeapDumpForm;
}

class RbkitMainWindow;

class HeapDumpForm : public QWidget
{
    Q_OBJECT
protected:
    QAction *viewRefAct;
    RBKit::HeapItem *rootItem;
    RBKit::HeapDataModel *model;
    SortObjectProxyModel *proxyModel;
    RBKit::HeapItem *selecteItem;
    RbkitMainWindow *parentWindow;
    bool disableRightClick;
public:
    explicit HeapDumpForm(QWidget *parent = 0, int _snapShotVersion = 0);
    ~HeapDumpForm();
    void loaData();
    void loadSelectedReferences(RBKit::HeapItem* _selectedItem);
    void adjustColumnWidth();
    RbkitMainWindow *getParentWindow() const;
    void setParentWindow(RbkitMainWindow *value);
    void setDisableRightClick(bool value);
    bool getDisableRightClick() const;
    void setTreeModel(SortObjectProxyModel* model);

    RBKit::HeapItem *getRootItem() const;

private:
    Ui::HeapDumpForm *ui;
    int snapShotVersion;
public slots:
    void onCustomContextMenu(const QPoint& point);
    void viewReferences();
    void treeNodeSelected(const QModelIndex& index);
};

typedef QSharedPointer<HeapDumpForm> HeapDumpFormPtr;

#endif // HEAPDUMPFORM_H
