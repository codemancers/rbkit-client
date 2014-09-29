#include "ui/rbkitmainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon(":/rbkit.icns"));

    RbkitMainWindow mainWindow;
    mainWindow.setWindowIcon(QIcon(":/rbkit.icns"));
    app.processEvents();
    mainWindow.show();

    return app.exec();
}
