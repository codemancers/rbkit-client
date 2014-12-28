#include "processdetail.h"

#include <QPushButton>
#include <QLabel>
#include "../model/appstate.h"
#include <QDebug>
#include <QHBoxLayout>

ProcessDetail::ProcessDetail()
{
    processInfoLabel = new QLabel();
    processInfoLabel->setStyleSheet("border-bottom: 2px; border-bottom-color: black");
    addWidget(processInfoLabel);
    displayProcessDetail();
}

ProcessDetail::~ProcessDetail()
{

}

void ProcessDetail::displayProcessDetail()
{
    if (RBKit::AppState::getInstance()->getState("process_name").isValid()) {
        processInfoLabel->setText("<b> Process Name </b> :" + RBKit::AppState::getInstance()->getState("process_name").toString() + "\n" +
                                  "<b> Process pid </b>: " + RBKit::AppState::getInstance()->getState("pid").toString() + "\n" +
                                  "<b> Process directory </b>: " + RBKit::AppState::getInstance()->getState("pwd").toString()
                              );
    } else {
        processInfoLabel->setText("Currently not profiling any Application");
    }
}

void ProcessDetail::createGenerationTable()
{
}
