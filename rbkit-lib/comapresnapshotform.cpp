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

void ComapreSnapshotForm::setSnapshotVersions(QList<int> _snapshotVersions) {
    snapshotVersions = _snapshotVersions;
    for(auto version : snapshotVersions) {
        ui->snapshot1->addItem(QString("Snapshot %0").arg(version));
        ui->snapshot2->addItem(QString("Snapshot %0").arg(version));
    }
}

QList<int> ComapreSnapshotForm::getSnapshotVersions() const {
    return snapshotVersions;
}

void ComapreSnapshotForm::on_okButton_clicked()
{
    QList<int> selections;
    int selection1 = ui->snapshot1->currentIndex();
    selections.append(snapshotVersions.at(selection1));
    int selection2 = ui->snapshot2->currentIndex();
    selections.append(snapshotVersions.at(selection2));
    emit snapshotSelected(selections);
}

void ComapreSnapshotForm::on_cancelButton_clicked()
{
    this->close();
}
