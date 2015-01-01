#ifndef RBKIT_MEMORYVIEW_H
#define RBKIT_MEMORYVIEW_H

#include <QWidget>
#include <QtWebKit>
#include <QWebFrame>

#include "model/jsbridge.h"

class ProcessDetail;

namespace RBKit {

namespace Ui {
class MemoryView;
}

class MemoryView : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryView(QWidget *parent = 0);
    ~MemoryView();

    RBKit::JsBridge *getJsBridge() const;
    void setJsBridge(RBKit::JsBridge *value);
    ProcessDetail *processDetail;
private:
    Ui::MemoryView *ui;
    RBKit::JsBridge *jsBridge;
private slots:
    void onPageLoad(bool ok);
};


} // namespace RBKit
#endif // RBKIT_MEMORYVIEW_H
