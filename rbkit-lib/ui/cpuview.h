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
    QStandardItemModel *model;
    QStandardItem *rootNode;
    explicit CpuView(QWidget *parent = 0);
    ~CpuView();

private:
    Ui::CpuView *ui;
};

typedef QSharedPointer<CpuView> CpuViewPtr;

#endif // CPUVIEW_H
