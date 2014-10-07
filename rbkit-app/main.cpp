#include "ui/rbkitmainwindow.h"
#include <QApplication>
#include "model/appstate.h"
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon(":/rbkit.icns"));

    RBKit::AppState::getInstance()->setAppState("connection_established", false);

    RbkitMainWindow mainWindow;
    mainWindow.setWindowIcon(QIcon(":/rbkit.icns"));
    app.processEvents();
    mainWindow.show();

    return app.exec();
}
