#ifndef PROCESSDETAIL_H
#define PROCESSDETAIL_H

#include <QVBoxLayout>
#include <QLabel>

class ProcessDetail : public QVBoxLayout
{
    QLabel *processInfoLabel;
public:
    ProcessDetail();
    ~ProcessDetail();

    void displayProcessDetail();
    void createGenerationTable();
};

#endif // PROCESSDETAIL_H
