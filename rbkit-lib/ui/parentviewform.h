#ifndef PARENTVIEWFORM_H
#define PARENTVIEWFORM_H

#include <QWidget>

namespace Ui {
class ParentViewForm;
}

class ParentViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit ParentViewForm(QWidget *parent = 0);
    ~ParentViewForm();

private:
    Ui::ParentViewForm *ui;
};

#endif // PARENTVIEWFORM_H
