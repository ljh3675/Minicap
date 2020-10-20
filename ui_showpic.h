/********************************************************************************
** Form generated from reading UI file 'showpic.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWPIC_H
#define UI_SHOWPIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_showpic
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton_startsever;
    QPushButton *pushButton_connect;
    QLabel *label_image_show;
    QLineEdit *lineEdit_IP;
    QTextBrowser *textBrowser_server;
    QTextBrowser *textBrowser_info;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *showpic)
    {
        if (showpic->objectName().isEmpty())
            showpic->setObjectName(QString::fromUtf8("showpic"));
        showpic->resize(800, 600);
        centralwidget = new QWidget(showpic);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton_startsever = new QPushButton(centralwidget);
        pushButton_startsever->setObjectName(QString::fromUtf8("pushButton_startsever"));
        pushButton_startsever->setGeometry(QRect(60, 70, 91, 23));
        pushButton_connect = new QPushButton(centralwidget);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));
        pushButton_connect->setGeometry(QRect(60, 110, 91, 23));
        label_image_show = new QLabel(centralwidget);
        label_image_show->setObjectName(QString::fromUtf8("label_image_show"));
        label_image_show->setGeometry(QRect(390, 20, 371, 521));
        lineEdit_IP = new QLineEdit(centralwidget);
        lineEdit_IP->setObjectName(QString::fromUtf8("lineEdit_IP"));
        lineEdit_IP->setGeometry(QRect(60, 30, 113, 20));
        textBrowser_server = new QTextBrowser(centralwidget);
        textBrowser_server->setObjectName(QString::fromUtf8("textBrowser_server"));
        textBrowser_server->setGeometry(QRect(50, 160, 261, 161));
        textBrowser_info = new QTextBrowser(centralwidget);
        textBrowser_info->setObjectName(QString::fromUtf8("textBrowser_info"));
        textBrowser_info->setGeometry(QRect(50, 350, 261, 192));
        showpic->setCentralWidget(centralwidget);
        menubar = new QMenuBar(showpic);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        showpic->setMenuBar(menubar);
        statusbar = new QStatusBar(showpic);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        showpic->setStatusBar(statusbar);

        retranslateUi(showpic);

        QMetaObject::connectSlotsByName(showpic);
    } // setupUi

    void retranslateUi(QMainWindow *showpic)
    {
        showpic->setWindowTitle(QCoreApplication::translate("showpic", "showpic", nullptr));
        pushButton_startsever->setText(QCoreApplication::translate("showpic", "\345\220\257\345\212\250 Minicap", nullptr));
        pushButton_connect->setText(QCoreApplication::translate("showpic", "\350\277\236\346\216\245", nullptr));
        label_image_show->setText(QCoreApplication::translate("showpic", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class showpic: public Ui_showpic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWPIC_H
