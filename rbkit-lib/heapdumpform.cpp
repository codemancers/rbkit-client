#include "heapdumpform.h"
#include "ui_heapdumpform.h"

HeapDumpForm::HeapDumpForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeapDumpForm)
{
    ui->setupUi(this);
}

HeapDumpForm::HeapDumpForm(QWidget *parent, const RBKit::ObjectStore &_objectStore) :
    HeapDumpForm(parent),
    objectStore(_objectStore)
{

}


HeapDumpForm::~HeapDumpForm()
{
    delete ui;
}
