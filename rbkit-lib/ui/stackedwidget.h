#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H
#include <QStackedWidget>
#include "appmainwindow.h"

class AppMainwindow;

class StackedWidget : public QStackedWidget
{
public:
    explicit StackedWidget(AppMainwindow* mainWindow);
};

#endif // STACKEDWIDGET_H
