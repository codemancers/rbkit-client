#ifndef COMAPRESNAPSHOTFORM_H
#define COMAPRESNAPSHOTFORM_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class ComapreSnapshotForm;
}

class ComapreSnapshotForm : public QDialog
{
    Q_OBJECT

public:
    explicit ComapreSnapshotForm(QWidget *parent = 0);
    ~ComapreSnapshotForm();

private:
    Ui::ComapreSnapshotForm *ui;
};

#endif // COMAPRESNAPSHOTFORM_H
