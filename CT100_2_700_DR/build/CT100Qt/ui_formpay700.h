/********************************************************************************
** Form generated from reading UI file 'formpay700.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMPAY700_H
#define UI_FORMPAY700_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Formpay700
{
public:
    QLabel *label_paymoney;
    QLabel *label_paynum;
    QLabel *label_paymode;
    QLabel *label_payqr;
    QLabel *label_paytit;

    void setupUi(QWidget *Formpay700)
    {
        if (Formpay700->objectName().isEmpty())
            Formpay700->setObjectName(QString::fromUtf8("Formpay700"));
        Formpay700->resize(800, 480);
        Formpay700->setMinimumSize(QSize(800, 480));
        Formpay700->setMaximumSize(QSize(800, 480));
        Formpay700->setStyleSheet(QString::fromUtf8("#Formpay700{border-image: url(:/image/430_pay.png);}"));
        label_paymoney = new QLabel(Formpay700);
        label_paymoney->setObjectName(QString::fromUtf8("label_paymoney"));
        label_paymoney->setGeometry(QRect(420, 350, 361, 81));
        label_paynum = new QLabel(Formpay700);
        label_paynum->setObjectName(QString::fromUtf8("label_paynum"));
        label_paynum->setGeometry(QRect(420, 200, 361, 71));
        label_paymode = new QLabel(Formpay700);
        label_paymode->setObjectName(QString::fromUtf8("label_paymode"));
        label_paymode->setGeometry(QRect(420, 60, 361, 71));
        label_payqr = new QLabel(Formpay700);
        label_payqr->setObjectName(QString::fromUtf8("label_payqr"));
        label_payqr->setGeometry(QRect(10, 60, 371, 361));
        label_payqr->setAutoFillBackground(false);
        label_paytit = new QLabel(Formpay700);
        label_paytit->setObjectName(QString::fromUtf8("label_paytit"));
        label_paytit->setGeometry(QRect(30, 26, 321, 31));
        label_paytit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        retranslateUi(Formpay700);

        QMetaObject::connectSlotsByName(Formpay700);
    } // setupUi

    void retranslateUi(QWidget *Formpay700)
    {
        Formpay700->setWindowTitle(QApplication::translate("Formpay700", "Form", 0, QApplication::UnicodeUTF8));
        label_paymoney->setText(QApplication::translate("Formpay700", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_paynum->setText(QApplication::translate("Formpay700", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_paymode->setText(QApplication::translate("Formpay700", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_payqr->setText(QApplication::translate("Formpay700", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_paytit->setText(QApplication::translate("Formpay700", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Formpay700: public Ui_Formpay700 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMPAY700_H
