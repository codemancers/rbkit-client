#ifndef HEAPDUMPFORM_H
#define HEAPDUMPFORM_H

#include <QWidget>
#include "objectstore.h"

namespace Ui {
class HeapDumpForm;
}

class HeapDumpForm : public QWidget
{
    Q_OBJECT

public:
    explicit HeapDumpForm(QWidget *parent = 0);
    HeapDumpForm(QWidget *parent = 0, const RBKit::ObjectStore &objectStore);
    ~HeapDumpForm();

private:
    Ui::HeapDumpForm *ui;
    const RBKit::ObjectStore objectStore;
};

#endif // HEAPDUMPFORM_H
