#ifndef HEAPDUMPFORM_H
#define HEAPDUMPFORM_H

#include <QWidget>
#include "objectstore.h"
#include <QTableView>
#include <heaptable.h>

namespace Ui {
class HeapDumpForm;
}

class HeapDumpForm : public QWidget
{
    Q_OBJECT

public:
    explicit HeapDumpForm(QWidget *parent = 0);
    ~HeapDumpForm();

    RBKit::ObjectStore getObjectStore() const;
    void setObjectStore(const RBKit::ObjectStore &value);

private:
    Ui::HeapDumpForm *ui;
    RBKit::ObjectStore objectStore;
};

#endif // HEAPDUMPFORM_H
