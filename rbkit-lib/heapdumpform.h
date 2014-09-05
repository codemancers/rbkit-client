#ifndef HEAPDUMPFORM_H
#define HEAPDUMPFORM_H

#include <QWidget>
#include <QTableView>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

#include "objectstore.h"
#include "heapdatamodel.h"
#include "heapitem.h"
#include "sqlconnectionpool.h"

namespace Ui {
class HeapDumpForm;
}

class HeapDumpForm : public QWidget
{
    Q_OBJECT

public:
    explicit HeapDumpForm(QWidget *parent = 0, int _snapShotVersion = 0);
    ~HeapDumpForm();
    void loaData();
private:
    Ui::HeapDumpForm *ui;
    int snapShotVersion;
};

#endif // HEAPDUMPFORM_H
