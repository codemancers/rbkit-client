#include "comparesnapshotform.h"
#include "ui_comparesnapshotform.h"
#include "model/appstate.h"

CompareSnapshotForm::CompareSnapshotForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompareSnapshotForm)
{
    ui->setupUi(this);
}

CompareSnapshotForm::~CompareSnapshotForm()
{
    delete ui;
}

void CompareSnapshotForm::setSnapshotVersions(QList<int> _snapshotVersions) {
    snapshotVersions = _snapshotVersions;
    for(auto version : snapshotVersions) {
        ui->snapshot1->addItem(RBKit::AppState::getInstance()->getSnapshotName(version));
        ui->snapshot2->addItem(RBKit::AppState::getInstance()->getSnapshotName(version));
    }
}

QList<int> CompareSnapshotForm::getSnapshotVersions() const {
    return snapshotVersions;
}

void CompareSnapshotForm::on_okButton_clicked()
{
    QList<int> selections;
    int selection1 = ui->snapshot1->currentIndex();
    selections.append(snapshotVersions.at(selection1));
    int selection2 = ui->snapshot2->currentIndex();
    selections.append(snapshotVersions.at(selection2));
    emit snapshotSelected(selections);
    this->close();
}

void CompareSnapshotForm::on_cancelButton_clicked()
{
    this->close();
}
