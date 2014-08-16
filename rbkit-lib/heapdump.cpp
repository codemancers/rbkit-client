#include "heapdump.h"
#include "ui_heapdump.h"

HeapDump::HeapDump(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeapDump)
{
    ui->setupUi(this);
}

HeapDump::~HeapDump()
{
    delete ui;
}
