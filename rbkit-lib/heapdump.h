#ifndef HEAPDUMP_H
#define HEAPDUMP_H

#include <QWidget>

namespace Ui {
class HeapDump;
}

class HeapDump : public QWidget
{
    Q_OBJECT

public:
    explicit HeapDump(QWidget *parent = 0);
    ~HeapDump();

private:
    Ui::HeapDump *ui;
};

#endif // HEAPDUMP_H
