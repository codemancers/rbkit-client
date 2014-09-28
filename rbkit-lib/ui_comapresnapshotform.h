/********************************************************************************
** Form generated from reading UI file 'comapresnapshotform.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMAPRESNAPSHOTFORM_H
#define UI_COMAPRESNAPSHOTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ComapreSnapshotForm
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *snapshot1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *snapshot2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *ComapreSnapshotForm)
    {
        if (ComapreSnapshotForm->objectName().isEmpty())
            ComapreSnapshotForm->setObjectName(QStringLiteral("ComapreSnapshotForm"));
        ComapreSnapshotForm->resize(348, 133);
        horizontalLayout_4 = new QHBoxLayout(ComapreSnapshotForm);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(ComapreSnapshotForm);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        snapshot1 = new QComboBox(ComapreSnapshotForm);
        snapshot1->setObjectName(QStringLiteral("snapshot1"));

        horizontalLayout->addWidget(snapshot1);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(ComapreSnapshotForm);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        snapshot2 = new QComboBox(ComapreSnapshotForm);
        snapshot2->setObjectName(QStringLiteral("snapshot2"));

        horizontalLayout_2->addWidget(snapshot2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        cancelButton = new QPushButton(ComapreSnapshotForm);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout_3->addWidget(cancelButton);

        okButton = new QPushButton(ComapreSnapshotForm);
        okButton->setObjectName(QStringLiteral("okButton"));

        horizontalLayout_3->addWidget(okButton);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout);


        retranslateUi(ComapreSnapshotForm);

        QMetaObject::connectSlotsByName(ComapreSnapshotForm);
    } // setupUi

    void retranslateUi(QDialog *ComapreSnapshotForm)
    {
        ComapreSnapshotForm->setWindowTitle(QApplication::translate("ComapreSnapshotForm", "Compare Snapshots", 0));
        label->setText(QApplication::translate("ComapreSnapshotForm", "Select Snapshot #1", 0));
        label_2->setText(QApplication::translate("ComapreSnapshotForm", "Select Snapshot #2", 0));
        cancelButton->setText(QApplication::translate("ComapreSnapshotForm", "Cancel", 0));
        okButton->setText(QApplication::translate("ComapreSnapshotForm", "Ok", 0));
    } // retranslateUi

};

namespace Ui {
    class ComapreSnapshotForm: public Ui_ComapreSnapshotForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMAPRESNAPSHOTFORM_H
