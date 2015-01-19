#ifndef COMPARESNAPSHOTFORM_H
#define COMPARESNAPSHOTFORM_H

#include <QWidget>
#include <QDialog>
#include <QList>

namespace Ui {
class CompareSnapshotForm;
}

class CompareSnapshotForm : public QDialog
{
    Q_OBJECT
    QList<int> snapshotVersions;

public:
    explicit CompareSnapshotForm(QWidget *parent = 0);
    ~CompareSnapshotForm();
    void setSnapshotVersions(QList<int> _snapshotVersions);
    QList<int> getSnapshotVersions() const;

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CompareSnapshotForm *ui;
signals:
    void snapshotSelected(QList<int> selected);
};

#endif // COMPARESNAPSHOTFORM_H
