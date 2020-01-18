/********************************************************************************
** Form generated from reading UI file 'formgoodskey700.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMGOODSKEY700_H
#define UI_FORMGOODSKEY700_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormGoodsKey700
{
public:
    QTableWidget *tableWidget;
    QLineEdit *lineEdit_goodsCode;
    QLineEdit *lineEdit_keys;
    QLabel *label_msg;
    QLabel *label_print;

    void setupUi(QWidget *FormGoodsKey700)
    {
        if (FormGoodsKey700->objectName().isEmpty())
            FormGoodsKey700->setObjectName(QString::fromUtf8("FormGoodsKey700"));
        FormGoodsKey700->resize(800, 480);
        FormGoodsKey700->setMinimumSize(QSize(800, 480));
        FormGoodsKey700->setMaximumSize(QSize(800, 480));
        FormGoodsKey700->setStyleSheet(QString::fromUtf8("#FormGoodsKey700{border-image: url(:/image/report.png);}"));
        tableWidget = new QTableWidget(FormGoodsKey700);
        if (tableWidget->columnCount() < 12)
            tableWidget->setColumnCount(12);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        if (tableWidget->rowCount() < 5)
            tableWidget->setRowCount(5);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem16);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 821, 381));
        tableWidget->horizontalHeader()->setDefaultSectionSize(66);
        tableWidget->verticalHeader()->setDefaultSectionSize(70);
        lineEdit_goodsCode = new QLineEdit(FormGoodsKey700);
        lineEdit_goodsCode->setObjectName(QString::fromUtf8("lineEdit_goodsCode"));
        lineEdit_goodsCode->setGeometry(QRect(10, 390, 381, 41));
        lineEdit_keys = new QLineEdit(FormGoodsKey700);
        lineEdit_keys->setObjectName(QString::fromUtf8("lineEdit_keys"));
        lineEdit_keys->setGeometry(QRect(400, 390, 391, 41));
        label_msg = new QLabel(FormGoodsKey700);
        label_msg->setObjectName(QString::fromUtf8("label_msg"));
        label_msg->setGeometry(QRect(20, 430, 621, 41));
        label_print = new QLabel(FormGoodsKey700);
        label_print->setObjectName(QString::fromUtf8("label_print"));
        label_print->setGeometry(QRect(580, 440, 211, 31));

        retranslateUi(FormGoodsKey700);

        QMetaObject::connectSlotsByName(FormGoodsKey700);
    } // setupUi

    void retranslateUi(QWidget *FormGoodsKey700)
    {
        FormGoodsKey700->setWindowTitle(QApplication::translate("FormGoodsKey700", "Form", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("FormGoodsKey700", "\347\254\2541\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("FormGoodsKey700", "\347\254\2542\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("FormGoodsKey700", "\347\254\2543\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("FormGoodsKey700", "\347\254\2544\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("FormGoodsKey700", "\347\254\2545\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("FormGoodsKey700", "\347\254\2546\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("FormGoodsKey700", "\347\254\2547\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("FormGoodsKey700", "\347\254\2548\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("FormGoodsKey700", "\347\254\2549\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("FormGoodsKey700", "\347\254\25410\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("FormGoodsKey700", "\347\254\25411\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QApplication::translate("FormGoodsKey700", "\347\254\25412\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem12->setText(QApplication::translate("FormGoodsKey700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem13->setText(QApplication::translate("FormGoodsKey700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem14->setText(QApplication::translate("FormGoodsKey700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem15->setText(QApplication::translate("FormGoodsKey700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem16->setText(QApplication::translate("FormGoodsKey700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        label_msg->setText(QApplication::translate("FormGoodsKey700", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_print->setText(QApplication::translate("FormGoodsKey700", "\346\211\223\345\215\260\345\225\206\345\223\201\347\274\226\347\240\201\346\230\216\347\273\206", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FormGoodsKey700: public Ui_FormGoodsKey700 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMGOODSKEY700_H
