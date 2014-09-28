/********************************************************************************
** Form generated from reading UI file 'askhost.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASKHOST_H
#define UI_ASKHOST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AskHost
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *hostInfo;
    QHBoxLayout *horizontalLayout;
    QLineEdit *eventsSocket;
    QLabel *label;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *commandsSocket;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *hostSelectedButton;

    void setupUi(QDialog *AskHost)
    {
        if (AskHost->objectName().isEmpty())
            AskHost->setObjectName(QStringLiteral("AskHost"));
        AskHost->setWindowModality(Qt::ApplicationModal);
        AskHost->resize(395, 194);
        AskHost->setMinimumSize(QSize(0, 100));
        verticalLayout_3 = new QVBoxLayout(AskHost);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        hostInfo = new QLabel(AskHost);
        hostInfo->setObjectName(QStringLiteral("hostInfo"));

        verticalLayout->addWidget(hostInfo);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        eventsSocket = new QLineEdit(AskHost);
        eventsSocket->setObjectName(QStringLiteral("eventsSocket"));

        horizontalLayout->addWidget(eventsSocket);


        verticalLayout->addLayout(horizontalLayout);

        label = new QLabel(AskHost);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        commandsSocket = new QLineEdit(AskHost);
        commandsSocket->setObjectName(QStringLiteral("commandsSocket"));

        horizontalLayout_3->addWidget(commandsSocket);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        hostSelectedButton = new QPushButton(AskHost);
        hostSelectedButton->setObjectName(QStringLiteral("hostSelectedButton"));

        horizontalLayout_2->addWidget(hostSelectedButton);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(AskHost);

        QMetaObject::connectSlotsByName(AskHost);
    } // setupUi

    void retranslateUi(QDialog *AskHost)
    {
        AskHost->setWindowTitle(QApplication::translate("AskHost", "Enter address of your Ruby app", 0));
        hostInfo->setText(QApplication::translate("AskHost", "Enter connection information for events", 0));
        eventsSocket->setText(QApplication::translate("AskHost", "tcp://127.0.0.1:5555", 0));
        label->setText(QApplication::translate("AskHost", "Enter connection information for commands", 0));
        commandsSocket->setText(QApplication::translate("AskHost", "tcp://127.0.0.1:5556", 0));
        hostSelectedButton->setText(QApplication::translate("AskHost", "&Ok", 0));
    } // retranslateUi

};

namespace Ui {
    class AskHost: public Ui_AskHost {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKHOST_H
