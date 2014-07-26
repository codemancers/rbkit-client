#include "askhost.h"
#include "ui_askhost.h"

AskHost::AskHost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskHost)
{
    ui->setupUi(this);
    connect(ui->hostSelectedButton, SIGNAL(clicked()), this, SLOT(userSelectedHost()));
}

AskHost::~AskHost()
{
    delete ui;
}

void AskHost::userSelectedHost() {
    selectedHost = ui->hostName->text();
    emit userHasSelectedHost(selectedHost);
}

