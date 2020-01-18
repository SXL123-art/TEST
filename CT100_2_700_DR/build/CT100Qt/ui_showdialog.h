/********************************************************************************
** Form generated from reading UI file 'showdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWDIALOG_H
#define UI_SHOWDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_showDialog
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_login;
    QWidget *tab_sale;
    QWidget *tab_pay;
    QWidget *tab_menu;
    QWidget *tab_cal;
    QWidget *tab_report;
    QWidget *tab_goodskey;

    void setupUi(QDialog *showDialog)
    {
        if (showDialog->objectName().isEmpty())
            showDialog->setObjectName(QString::fromUtf8("showDialog"));
        showDialog->resize(512, 290);
        gridLayout = new QGridLayout(showDialog);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(showDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setAutoFillBackground(false);
        tabWidget->setUsesScrollButtons(false);
        tab_login = new QWidget();
        tab_login->setObjectName(QString::fromUtf8("tab_login"));
        tabWidget->addTab(tab_login, QString());
        tab_sale = new QWidget();
        tab_sale->setObjectName(QString::fromUtf8("tab_sale"));
        tabWidget->addTab(tab_sale, QString());
        tab_pay = new QWidget();
        tab_pay->setObjectName(QString::fromUtf8("tab_pay"));
        tabWidget->addTab(tab_pay, QString());
        tab_menu = new QWidget();
        tab_menu->setObjectName(QString::fromUtf8("tab_menu"));
        tabWidget->addTab(tab_menu, QString());
        tab_cal = new QWidget();
        tab_cal->setObjectName(QString::fromUtf8("tab_cal"));
        tabWidget->addTab(tab_cal, QString());
        tab_report = new QWidget();
        tab_report->setObjectName(QString::fromUtf8("tab_report"));
        tabWidget->addTab(tab_report, QString());
        tab_goodskey = new QWidget();
        tab_goodskey->setObjectName(QString::fromUtf8("tab_goodskey"));
        tabWidget->addTab(tab_goodskey, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(showDialog);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(showDialog);
    } // setupUi

    void retranslateUi(QDialog *showDialog)
    {
        showDialog->setWindowTitle(QApplication::translate("showDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_login), QApplication::translate("showDialog", "\347\231\273\345\275\225\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_sale), QApplication::translate("showDialog", "\351\224\200\345\224\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_pay), QApplication::translate("showDialog", "\346\224\257\344\273\230\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_menu), QApplication::translate("showDialog", "\350\256\276\347\275\256\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_cal), QApplication::translate("showDialog", "\344\270\200\351\224\256\346\240\207\345\256\232", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_report), QApplication::translate("showDialog", "\346\212\245\350\241\250\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_goodskey), QApplication::translate("showDialog", "\345\225\206\345\223\201\346\214\211\351\224\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class showDialog: public Ui_showDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWDIALOG_H
