/********************************************************************************
** Form generated from reading UI file 'main_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_DIALOG_H
#define UI_MAIN_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QLabel *titleLabel;
    QPushButton *createBtn;
    QPushButton *connectBtn;
    QLabel *lineLabel;
    QLabel *copyrightLabel;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QString::fromUtf8("MainDialog"));
        MainDialog->resize(354, 388);
        MainDialog->setStyleSheet(QString::fromUtf8("QDialog {\n"
"background-color: '#0d3d4c';\n"
"}"));
        titleLabel = new QLabel(MainDialog);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setGeometry(QRect(70, 20, 221, 41));
        titleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 700 30pt \"Yu Gothic UI\";\n"
"	letter-spacing: 1px;\n"
"	color: '#e3e3e3';\n"
"}"));
        titleLabel->setAlignment(Qt::AlignCenter);
        createBtn = new QPushButton(MainDialog);
        createBtn->setObjectName(QString::fromUtf8("createBtn"));
        createBtn->setGeometry(QRect(80, 130, 201, 61));
        createBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	border: 5px solid '#101014';\n"
"	font: 700 20pt \"Yu Gothic UI\";\n"
"	letter-spacing: 1.4px;\n"
"	color: '#e3e3e3';\n"
"	border-radius: 12px;\n"
"}\n"
"\n"
"QPushButton::hover {\n"
"	background-color: '#0c2f3a';\n"
"}"));
        connectBtn = new QPushButton(MainDialog);
        connectBtn->setObjectName(QString::fromUtf8("connectBtn"));
        connectBtn->setGeometry(QRect(80, 230, 201, 61));
        connectBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	border: 5px solid '#101014';\n"
"	font: 700 20pt \"Yu Gothic UI\";\n"
"	letter-spacing: 1.4px;\n"
"	color: '#e3e3e3';\n"
"	border-radius: 12px;\n"
"}\n"
"\n"
"QPushButton::hover {\n"
"	background-color: '#0c2f3a';\n"
"}"));
        lineLabel = new QLabel(MainDialog);
        lineLabel->setObjectName(QString::fromUtf8("lineLabel"));
        lineLabel->setGeometry(QRect(80, 70, 201, 4));
        lineLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: '#101014';\n"
"	border-radius: 2px;\n"
"}"));
        copyrightLabel = new QLabel(MainDialog);
        copyrightLabel->setObjectName(QString::fromUtf8("copyrightLabel"));
        copyrightLabel->setGeometry(QRect(30, 364, 291, 21));
        copyrightLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font: 700 8pt \"Yu Gothic UI\";\n"
"	color: rgb(165, 165, 153);\n"
"}"));

        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QCoreApplication::translate("MainDialog", "Multi Chat", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainDialog", "Multi Chat", nullptr));
        createBtn->setText(QCoreApplication::translate("MainDialog", "Create", nullptr));
        connectBtn->setText(QCoreApplication::translate("MainDialog", "Connect", nullptr));
        lineLabel->setText(QString());
        copyrightLabel->setText(QCoreApplication::translate("MainDialog", "Copyright \302\251 2022 Vincenzo Caliendo. All rights reserved", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_DIALOG_H
