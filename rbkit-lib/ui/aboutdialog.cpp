#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->logoFrame->setFrameStyle(QFrame::StyledPanel);
    ui->logoFrame->setStyleSheet("background-image: url(:/icons/rbkitlogo-128.png); background-repeat: no-repeat; border: 0px;");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
    this->close();
}
