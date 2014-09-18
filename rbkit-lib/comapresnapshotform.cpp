#include "comapresnapshotform.h"
#include "ui_comapresnapshotform.h"

ComapreSnapshotForm::ComapreSnapshotForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComapreSnapshotForm)
{
    ui->setupUi(this);
}

ComapreSnapshotForm::~ComapreSnapshotForm()
{
    delete ui;
}


void ComapreSnapshotForm::on_okButton_clicked()
{

}

void ComapreSnapshotForm::on_cancelButton_clicked()
{

}
