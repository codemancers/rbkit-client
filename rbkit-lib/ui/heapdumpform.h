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
#include "model/heap_item_types/baseheapitem.h"
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
    QAction *viewParentsAct;
    QAction *viewFileAct;
    RBKit::BaseHeapItem *rootItem;
    RBKit::HeapDataModel *model;
    SortObjectProxyModel *proxyModel;
    RBKit::BaseHeapItem *selecteItem;
    RbkitMainWindow *parentWindow;
    bool disableRightClick;
public:
    explicit HeapDumpForm(QWidget *parent = 0, int _snapShotVersion = 0);
    virtual ~HeapDumpForm();
    void loaData();
    void loadSelectedReferences(RBKit::BaseHeapItem* _selectedItem);
    void adjustColumnWidth();
    void loadFromSpecifiedRoot(RBKit::BaseHeapItem*_rootItem);
    RbkitMainWindow *getParentWindow() const;
    void setParentWindow(RbkitMainWindow *value);
    void setDisableRightClick(bool value);
    bool getDisableRightClick() const;
    void setTreeModel(SortObjectProxyModel* model);
    void reset();

    RBKit::BaseHeapItem *getRootItem() const;
    Ui::HeapDumpForm *ui;
private:
    int snapShotVersion;
public slots:
    void onCustomContextMenu(const QPoint& point);
    void viewReferences();
    void viewParents();
    void viewFile();
    virtual void treeNodeSelected(const QModelIndex& index);
};

typedef QSharedPointer<HeapDumpForm> HeapDumpFormPtr;

#endif // HEAPDUMPFORM_H
