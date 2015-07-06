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
    QString serverHost = ui->serverHost->text();
    QString commandsSocket = serverHost + ":" + ui->commandsPort->text();
    QString eventsSocket = serverHost + ":" + ui->eventsPort->text();

    qDebug() << commandsSocket << eventsSocket;
    emit userHasSelectedHost(commandsSocket, eventsSocket);
}
