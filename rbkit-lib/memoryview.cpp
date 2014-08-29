#include "memoryview.h"
#include "ui_memoryview.h"

namespace RBKit {

MemoryView::MemoryView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemoryView)
{
    ui->setupUi(this);
    QWebSettings *settings = ui->chartingView->settings();
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::AutoLoadImages, true);
    settings->setMaximumPagesInCache(0);
    settings->setObjectCacheCapacities(0, 0, 0);
    settings->setIconDatabasePath("");

    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::PluginsEnabled, false);
    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::PrivateBrowsingEnabled, false);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);

    connect(ui->chartingView, SIGNAL(loadFinished(bool)), this, SLOT(onPageLoad(bool)));
    ui->chartingView->setUrl(QUrl("qrc:/web/index.html"));

    jsBridge = new RBKit::JsBridge();
}

MemoryView::~MemoryView()
{
    delete jsBridge;
    delete ui;
}
RBKit::JsBridge *MemoryView::getJsBridge() const
{
    return jsBridge;
}

void MemoryView::setJsBridge(RBKit::JsBridge *value)
{
    jsBridge = value;
}


void MemoryView::onPageLoad(bool ok)
{
    QWebFrame *frame = ui->chartingView->page()->mainFrame();
    frame->addToJavaScriptWindowObject(QString("jsBridge"), jsBridge);
}

} // namespace RBKit
