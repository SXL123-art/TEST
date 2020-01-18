/********************************************************************************
** Form generated from reading UI file 'addsaledialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDSALEDIALOG_H
#define UI_ADDSALEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_AddSaleDialog
{
public:
    QLabel *label_title;
    QLabel *label_msg;
    QLabel *label_name;
    QLabel *label_discount;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_weight;
    QLabel *label_price;
    QLabel *label_totel;

    void setupUi(QDialog *AddSaleDialog)
    {
        if (AddSaleDialog->objectName().isEmpty())
            AddSaleDialog->setObjectName(QString::fromUtf8("AddSaleDialog"));
        AddSaleDialog->resize(663, 335);
        AddSaleDialog->setStyleSheet(QString::fromUtf8("#AddSaleDialog{border-image: url(:/image/dialog.png);}"));
        label_title = new QLabel(AddSaleDialog);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setGeometry(QRect(20, 15, 571, 41));
        label_msg = new QLabel(AddSaleDialog);
        label_msg->setObjectName(QString::fromUtf8("label_msg"));
        label_msg->setGeometry(QRect(10, 195, 631, 41));
        label_name = new QLabel(AddSaleDialog);
        label_name->setObjectName(QString::fromUtf8("label_name"));
        label_name->setGeometry(QRect(102, 82, 241, 41));
        label_discount = new QLabel(AddSaleDialog);
        label_discount->setObjectName(QString::fromUtf8("label_discount"));
        label_discount->setGeometry(QRect(10, 257, 291, 41));
        label = new QLabel(AddSaleDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 140, 81, 51));
        label_2 = new QLabel(AddSaleDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 82, 81, 41));
        label_3 = new QLabel(AddSaleDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(350, 80, 81, 51));
        label_4 = new QLabel(AddSaleDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(350, 140, 81, 51));
        label_weight = new QLabel(AddSaleDialog);
        label_weight->setObjectName(QString::fromUtf8("label_weight"));
        label_weight->setGeometry(QRect(104, 145, 241, 41));
        label_price = new QLabel(AddSaleDialog);
        label_price->setObjectName(QString::fromUtf8("label_price"));
        label_price->setGeometry(QRect(440, 85, 211, 41));
        label_totel = new QLabel(AddSaleDialog);
        label_totel->setObjectName(QString::fromUtf8("label_totel"));
        label_totel->setGeometry(QRect(440, 145, 211, 41));

        retranslateUi(AddSaleDialog);

        QMetaObject::connectSlotsByName(AddSaleDialog);
    } // setupUi

    void retranslateUi(QDialog *AddSaleDialog)
    {
        AddSaleDialog->setWindowTitle(QApplication::translate("AddSaleDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_title->setText(QApplication::translate("AddSaleDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_msg->setText(QApplication::translate("AddSaleDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_name->setText(QApplication::translate("AddSaleDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_discount->setText(QApplication::translate("AddSaleDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AddSaleDialog", "\346\225\260\351\207\217:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AddSaleDialog", "\345\225\206\345\223\201:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AddSaleDialog", "\345\215\225\344\273\267:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AddSaleDialog", "\346\200\273\344\273\267:", 0, QApplication::UnicodeUTF8));
        label_weight->setText(QApplication::translate("AddSaleDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_price->setText(QApplication::translate("AddSaleDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_totel->setText(QApplication::translate("AddSaleDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AddSaleDialog: public Ui_AddSaleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDSALEDIALOG_H
