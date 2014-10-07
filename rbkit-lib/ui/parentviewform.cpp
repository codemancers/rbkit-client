#include "parentviewform.h"
#include "ui_parentviewform.h"

ParentViewForm::ParentViewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParentViewForm)
{
    ui->setupUi(this);
}

ParentViewForm::~ParentViewForm()
{
    delete ui;
}
