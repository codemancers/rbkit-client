#ifndef CPUVIEW_H
#define CPUVIEW_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class CpuView;
}

class CpuView : public QWidget
{
    Q_OBJECT

public:
    explicit CpuView(QWidget *parent = 0);
    ~CpuView();
    void updateCpuModel(QStandardItemModel *model);

private slots:
    void on_callGraphRadio_clicked();

    void on_flatRadio_clicked();

public:
signals:
    void traverseCallGraph();
    void traverseFlatProfile();

private:
    Ui::CpuView *ui;
};

#endif // CPUVIEW_H
