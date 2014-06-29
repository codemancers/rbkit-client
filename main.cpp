#include "rbkitmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    RbkitMainWindow mainWindow;
    app.processEvents();
    mainWindow.show();

    return app.exec();
}
