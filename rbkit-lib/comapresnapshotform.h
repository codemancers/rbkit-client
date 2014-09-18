#ifndef COMAPRESNAPSHOTFORM_H
#define COMAPRESNAPSHOTFORM_H

#include <QWidget>
#include <QDialog>
#include <QList>

namespace Ui {
class ComapreSnapshotForm;
}

class ComapreSnapshotForm : public QDialog
{
    Q_OBJECT
    QList<int> snapshotVersions;

public:
    explicit ComapreSnapshotForm(QWidget *parent = 0);
    ~ComapreSnapshotForm();
    void setSnapshotVersions(QList<int> _snapshotVersions);
    QList<int> getSnapshotVersions() const;

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ComapreSnapshotForm *ui;
};

#endif // COMAPRESNAPSHOTFORM_H
