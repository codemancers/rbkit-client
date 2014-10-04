#ifndef ACTIONTOOLBAR_H
#define ACTIONTOOLBAR_H

#include <QToolBar>

namespace Ui {
class RbkitMainWindow;
}

// Maintains state of Toolbar
class ActionToolbar
{
public:
    ActionToolbar(Ui::RbkitMainWindow *ui);
    void enableProfileActions();
    void disableProfileActions();
private:
    Ui::RbkitMainWindow *ui;
};

#endif // ACTIONTOOLBAR_H
