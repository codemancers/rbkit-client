#ifndef CPUVIEW_H
#define CPUVIEW_H

#include <QWidget>
#include <QStandardItemModel>
//#include "appmainwindow.h"

namespace Ui {
class CpuView;
}

class CpuView : public QWidget
{
    Q_OBJECT

public:
    QStandardItemModel *callGraphModel;
    QStandardItemModel *flatGraphModel;

    explicit CpuView(QWidget *parent = 0);
    ~CpuView();
    void updateCpuModel(QStandardItemModel *model);

private slots:
    void on_callGraphRadio_clicked();
    void on_flatRadio_clicked();

public:
signals:
    void fillCallGraph(QStandardItemModel*);
    void fillFlatProfile(QStandardItemModel*);

private:
    Ui::CpuView *ui;
};

#endif // CPUVIEW_H
