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

#include "objectstore.h"
#include "heapdatamodel.h"
#include "heapitem.h"
#include "sqlconnectionpool.h"
#include "sortobjectproxymodel.h"

namespace Ui {
class HeapDumpForm;
}

class RbkitMainWindow;

class HeapDumpForm : public QWidget
{
    Q_OBJECT
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

private:
    Ui::HeapDumpForm *ui;
    int snapShotVersion;
public slots:
    void onCustomContextMenu(const QPoint& point);
    void viewReferences();
    void treeNodeSelected(const QModelIndex& index);
};

#endif // HEAPDUMPFORM_H
