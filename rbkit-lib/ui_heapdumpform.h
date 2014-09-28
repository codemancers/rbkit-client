/********************************************************************************
** Form generated from reading UI file 'heapdumpform.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEAPDUMPFORM_H
#define UI_HEAPDUMPFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HeapDumpForm
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;

    void setupUi(QWidget *HeapDumpForm)
    {
        if (HeapDumpForm->objectName().isEmpty())
            HeapDumpForm->setObjectName(QStringLiteral("HeapDumpForm"));
        HeapDumpForm->resize(678, 416);
        horizontalLayout = new QHBoxLayout(HeapDumpForm);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        treeView = new QTreeView(HeapDumpForm);
        treeView->setObjectName(QStringLiteral("treeView"));

        horizontalLayout->addWidget(treeView);


        retranslateUi(HeapDumpForm);

        QMetaObject::connectSlotsByName(HeapDumpForm);
    } // setupUi

    void retranslateUi(QWidget *HeapDumpForm)
    {
        HeapDumpForm->setWindowTitle(QApplication::translate("HeapDumpForm", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class HeapDumpForm: public Ui_HeapDumpForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEAPDUMPFORM_H
