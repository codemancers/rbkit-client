#ifndef PROCESSDETAIL_H
#define PROCESSDETAIL_H

#include <QVBoxLayout>
#include <QLabel>
#include <QVariantMap>
#include <QVector>

class QTableView;
class QWidget;
class QHBoxLayout;
class QStandardItemModel;
class  QSortFilterProxyModel;

class ProcessDetail : public QVBoxLayout
{
    QLabel *processInfoLabel;
    QVector<QWidget *> widgets;
    QHBoxLayout *horizontalTables;
    QTableView *youngView;
    QTableView *secondGenView;
    QTableView *oldView;


    QStandardItemModel *youngModel;
    QStandardItemModel *secondGenModel;
    QStandardItemModel *oldGenModel;
public:
    ProcessDetail();
    ~ProcessDetail();

    void displayProcessDetail();
    void createGenerationTable();
    QTableView *createTableView(const QString &label);
    void receiveGenerationStats(int genType, QVariantMap map);
    void createTableHeader(QStandardItemModel &model);
    QStandardItemModel* createModel(QTableView& tableView);
    void updateModel(QStandardItemModel* model, QVariantMap &map);
    void fixTableDisplay(QTableView *view);
    void disconnectedFromProcess();

};

#endif // PROCESSDETAIL_H
