/********************************************************************************
** Form generated from reading UI file 'loginform700.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINFORM700_H
#define UI_LOGINFORM700_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginForm700
{
public:
    QLabel *label_swv;
    QLabel *label_msg;
    QLineEdit *lineEdit_psw;
    QLineEdit *lineEdit_user;
    QLabel *label_2;
    QLabel *label_weight;
    QLabel *label_hdv;
    QLabel *label_tare;
    QLabel *label_1;

    void setupUi(QWidget *LoginForm700)
    {
        if (LoginForm700->objectName().isEmpty())
            LoginForm700->setObjectName(QString::fromUtf8("LoginForm700"));
        LoginForm700->resize(800, 480);
        LoginForm700->setMinimumSize(QSize(800, 480));
        LoginForm700->setMaximumSize(QSize(800, 480));
        LoginForm700->setStyleSheet(QString::fromUtf8("#LoginForm700{border-image: url(:/image/login.png);}"));
        label_swv = new QLabel(LoginForm700);
        label_swv->setObjectName(QString::fromUtf8("label_swv"));
        label_swv->setGeometry(QRect(530, 10, 261, 17));
        label_swv->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_msg = new QLabel(LoginForm700);
        label_msg->setObjectName(QString::fromUtf8("label_msg"));
        label_msg->setGeometry(QRect(150, 390, 631, 41));
        label_msg->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_psw = new QLineEdit(LoginForm700);
        lineEdit_psw->setObjectName(QString::fromUtf8("lineEdit_psw"));
        lineEdit_psw->setGeometry(QRect(270, 300, 511, 51));
        lineEdit_user = new QLineEdit(LoginForm700);
        lineEdit_user->setObjectName(QString::fromUtf8("lineEdit_user"));
        lineEdit_user->setGeometry(QRect(270, 230, 511, 51));
        label_2 = new QLabel(LoginForm700);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 300, 121, 41));
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_weight = new QLabel(LoginForm700);
        label_weight->setObjectName(QString::fromUtf8("label_weight"));
        label_weight->setGeometry(QRect(580, 110, 191, 71));
        label_hdv = new QLabel(LoginForm700);
        label_hdv->setObjectName(QString::fromUtf8("label_hdv"));
        label_hdv->setGeometry(QRect(150, 10, 241, 17));
        label_hdv->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_tare = new QLabel(LoginForm700);
        label_tare->setObjectName(QString::fromUtf8("label_tare"));
        label_tare->setGeometry(QRect(300, 110, 211, 71));
        label_1 = new QLabel(LoginForm700);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(70, 230, 151, 41));
        label_1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        retranslateUi(LoginForm700);

        QMetaObject::connectSlotsByName(LoginForm700);
    } // setupUi

    void retranslateUi(QWidget *LoginForm700)
    {
        LoginForm700->setWindowTitle(QApplication::translate("LoginForm700", "Form", 0, QApplication::UnicodeUTF8));
        label_swv->setText(QApplication::translate("LoginForm700", "--------------", 0, QApplication::UnicodeUTF8));
        label_msg->setText(QApplication::translate("LoginForm700", "--------------------", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LoginForm700", "\345\257\206  \347\240\201:", 0, QApplication::UnicodeUTF8));
        label_weight->setText(QApplication::translate("LoginForm700", "000000", 0, QApplication::UnicodeUTF8));
        label_hdv->setText(QApplication::translate("LoginForm700", "--------------", 0, QApplication::UnicodeUTF8));
        label_tare->setText(QApplication::translate("LoginForm700", "000000", 0, QApplication::UnicodeUTF8));
        label_1->setText(QApplication::translate("LoginForm700", "\347\224\250\346\210\267\345\220\215:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LoginForm700: public Ui_LoginForm700 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFORM700_H
