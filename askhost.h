#ifndef ASKHOST_H
#define ASKHOST_H

#include <QWidget>

namespace Ui {
class AskHost;
}

class AskHost : public QWidget
{
    Q_OBJECT
    QString selectedHost;

public:
    explicit AskHost(QWidget *parent = 0);
    ~AskHost();

private slots:
    void userSelectedHost();

signals:
    void userHasSelectedHost(const QString&);

private:
    Ui::AskHost *ui;
};

#endif // ASKHOST_H
