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
    QPushButton *pushButton_connect;
    QPushButton *pushButton_save;
    QLabel *label_image_show;
    QLineEdit *lineEdit_IP;
    QTextBrowser *textBrowser;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *showpic)
    {
        if (showpic->objectName().isEmpty())
            showpic->setObjectName(QString::fromUtf8("showpic"));
        showpic->resize(800, 600);
        centralwidget = new QWidget(showpic);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton_connect = new QPushButton(centralwidget);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));
        pushButton_connect->setGeometry(QRect(60, 210, 75, 23));
        pushButton_save = new QPushButton(centralwidget);
        pushButton_save->setObjectName(QString::fromUtf8("pushButton_save"));
        pushButton_save->setGeometry(QRect(60, 250, 75, 23));
        label_image_show = new QLabel(centralwidget);
        label_image_show->setObjectName(QString::fromUtf8("label_image_show"));
        label_image_show->setGeometry(QRect(390, 10, 361, 521));
        lineEdit_IP = new QLineEdit(centralwidget);
        lineEdit_IP->setObjectName(QString::fromUtf8("lineEdit_IP"));
        lineEdit_IP->setGeometry(QRect(50, 120, 113, 20));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(50, 330, 256, 192));
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
        pushButton_connect->setText(QCoreApplication::translate("showpic", "connect", nullptr));
        pushButton_save->setText(QCoreApplication::translate("showpic", "save", nullptr));
        label_image_show->setText(QCoreApplication::translate("showpic", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class showpic: public Ui_showpic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWPIC_H
