#include <QDebug>
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

void AskHost::userSelectedHost()
{
    QString commandsSocket = ui->commandsSocket->text();
    QString eventsSocket = ui->eventsSocket->text();

    qDebug() << commandsSocket << eventsSocket;
    emit userHasSelectedHost(commandsSocket, eventsSocket);
}
