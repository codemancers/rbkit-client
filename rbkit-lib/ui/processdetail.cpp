#include "processdetail.h"

#include <QPushButton>
#include <QLabel>
#include "../model/appstate.h"
#include <QDebug>
#include <QHBoxLayout>
    #include <QTableView>

ProcessDetail::ProcessDetail()
{
    processInfoLabel = new QLabel();
    processInfoLabel->setStyleSheet("border-bottom: 2px; border-bottom-color: black");
    addWidget(processInfoLabel);
    displayProcessDetail();
    createGenerationTable();
}

ProcessDetail::~ProcessDetail()
{

}

void ProcessDetail::displayProcessDetail()
{
    if (RBKit::AppState::getInstance()->getState("process_name").isValid()) {
        processInfoLabel->setText("<b> Process Name </b> :" + RBKit::AppState::getInstance()->getState("process_name").toString() + "<br />" +
                                  "<b> Process pid </b>: " + RBKit::AppState::getInstance()->getState("pid").toString() + "<br />" +
                                  "<b> Process directory </b>: " + RBKit::AppState::getInstance()->getState("pwd").toString()
                              );
    } else {
        processInfoLabel->setText("Currently not profiling any Application");
    }
}

void ProcessDetail::createGenerationTable()
{
    QWidget *widget = new QWidget();
    QHBoxLayout *horizontalTables = new QHBoxLayout();
    horizontalTables->setMargin(0);

    QWidget *gc1 = new QWidget();
    QVBoxLayout *gc1Layout = new QVBoxLayout();
    gc1Layout->setMargin(0);
    gc1->setLayout(gc1Layout);

    QLabel *gc1Label = new QLabel("Young Generation");
    gc1Layout->addWidget(gc1Label);
    QTableView *view1 = new QTableView();
    gc1Layout->addWidget(view1);
    horizontalTables->addWidget(gc1);



    QWidget *gc2 = new QWidget();
    QVBoxLayout *gc2Layout = new QVBoxLayout();
    gc2Layout->setMargin(0);
    gc2->setLayout(gc2Layout);

    QLabel *gc2Label = new QLabel("Medium Generation");
    gc2Layout->addWidget(gc2Label);
    QTableView *view2 = new QTableView();
    gc2Layout->addWidget(view2);
    horizontalTables->addWidget(gc2);



    QWidget *gc3 = new QWidget();
    QVBoxLayout *gc3Layout = new QVBoxLayout();
    gc3Layout->setMargin(0);
    gc3->setLayout(gc3Layout);

    QLabel *gc3Label = new QLabel("Old Generation");
    gc3Layout->addWidget(gc3Label);
    QTableView *view3 = new QTableView();
    gc3Layout->addWidget(view3);
    horizontalTables->addWidget(gc3);

    widget->setLayout(horizontalTables);

    addWidget(widget);
}
