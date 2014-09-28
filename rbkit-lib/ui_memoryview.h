/********************************************************************************
** Form generated from reading UI file 'memoryview.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEMORYVIEW_H
#define UI_MEMORYVIEW_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace RBKit {

class Ui_MemoryView
{
public:
    QVBoxLayout *verticalLayout;
    QWebView *chartingView;

    void setupUi(QWidget *RBKit__MemoryView)
    {
        if (RBKit__MemoryView->objectName().isEmpty())
            RBKit__MemoryView->setObjectName(QStringLiteral("RBKit__MemoryView"));
        RBKit__MemoryView->resize(400, 300);
        verticalLayout = new QVBoxLayout(RBKit__MemoryView);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        chartingView = new QWebView(RBKit__MemoryView);
        chartingView->setObjectName(QStringLiteral("chartingView"));
        chartingView->setUrl(QUrl(QStringLiteral("about:blank")));

        verticalLayout->addWidget(chartingView);


        retranslateUi(RBKit__MemoryView);

        QMetaObject::connectSlotsByName(RBKit__MemoryView);
    } // setupUi

    void retranslateUi(QWidget *RBKit__MemoryView)
    {
        RBKit__MemoryView->setWindowTitle(QApplication::translate("RBKit::MemoryView", "Form", 0));
    } // retranslateUi

};

} // namespace RBKit

namespace RBKit {
namespace Ui {
    class MemoryView: public Ui_MemoryView {};
} // namespace Ui
} // namespace RBKit

#endif // UI_MEMORYVIEW_H
