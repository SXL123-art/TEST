/********************************************************************************
** Form generated from reading UI file 'formreport700.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMREPORT700_H
#define UI_FORMREPORT700_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormReport700
{
public:
    QTableWidget *tableWidget;
    QLabel *label_title;
    QLabel *label_next;
    QLabel *label_prior;
    QLabel *label_num;
    QTableWidget *tableWidget_item;
    QLabel *label_print;

    void setupUi(QWidget *FormReport700)
    {
        if (FormReport700->objectName().isEmpty())
            FormReport700->setObjectName(QString::fromUtf8("FormReport700"));
        FormReport700->resize(800, 480);
        FormReport700->setMinimumSize(QSize(800, 480));
        FormReport700->setMaximumSize(QSize(800, 480));
        FormReport700->setStyleSheet(QString::fromUtf8("#FormReport700{border-image: url(:/image/report.png);}"));
        tableWidget = new QTableWidget(FormReport700);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (tableWidget->rowCount() < 11)
            tableWidget->setRowCount(11);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(5, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(6, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(7, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(8, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(9, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(10, __qtablewidgetitem14);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 611, 481));
        tableWidget->horizontalHeader()->setDefaultSectionSize(140);
        tableWidget->horizontalHeader()->setMinimumSectionSize(31);
        tableWidget->verticalHeader()->setDefaultSectionSize(41);
        label_title = new QLabel(FormReport700);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setGeometry(QRect(610, 390, 191, 21));
        label_title->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_next = new QLabel(FormReport700);
        label_next->setObjectName(QString::fromUtf8("label_next"));
        label_next->setGeometry(QRect(610, 450, 91, 21));
        label_prior = new QLabel(FormReport700);
        label_prior->setObjectName(QString::fromUtf8("label_prior"));
        label_prior->setGeometry(QRect(610, 420, 91, 20));
        label_num = new QLabel(FormReport700);
        label_num->setObjectName(QString::fromUtf8("label_num"));
        label_num->setGeometry(QRect(700, 449, 101, 21));
        tableWidget_item = new QTableWidget(FormReport700);
        if (tableWidget_item->columnCount() < 1)
            tableWidget_item->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_item->setHorizontalHeaderItem(0, __qtablewidgetitem15);
        if (tableWidget_item->rowCount() < 4)
            tableWidget_item->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_item->setVerticalHeaderItem(0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_item->setVerticalHeaderItem(1, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_item->setVerticalHeaderItem(2, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_item->setVerticalHeaderItem(3, __qtablewidgetitem19);
        tableWidget_item->setObjectName(QString::fromUtf8("tableWidget_item"));
        tableWidget_item->setGeometry(QRect(597, 0, 201, 391));
        tableWidget_item->horizontalHeader()->setDefaultSectionSize(149);
        tableWidget_item->verticalHeader()->setDefaultSectionSize(91);
        label_print = new QLabel(FormReport700);
        label_print->setObjectName(QString::fromUtf8("label_print"));
        label_print->setGeometry(QRect(700, 419, 101, 21));

        retranslateUi(FormReport700);

        QMetaObject::connectSlotsByName(FormReport700);
    } // setupUi

    void retranslateUi(QWidget *FormReport700)
    {
        FormReport700->setWindowTitle(QApplication::translate("FormReport700", "Form", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("FormReport700", "\345\225\206\345\223\201\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("FormReport700", "\351\207\215\351\207\217", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("FormReport700", "\346\200\273\344\273\267", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("FormReport700", "\346\224\257\344\273\230\346\226\271\345\274\217", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem7->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem8->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem9->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->verticalHeaderItem(6);
        ___qtablewidgetitem10->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->verticalHeaderItem(7);
        ___qtablewidgetitem11->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->verticalHeaderItem(8);
        ___qtablewidgetitem12->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget->verticalHeaderItem(9);
        ___qtablewidgetitem13->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget->verticalHeaderItem(10);
        ___qtablewidgetitem14->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        label_title->setText(QApplication::translate("FormReport700", "\350\257\267\351\200\211\346\213\251\344\270\212\345\210\227\346\212\245\350\241\250\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        label_next->setText(QApplication::translate("FormReport700", "-----", 0, QApplication::UnicodeUTF8));
        label_prior->setText(QApplication::translate("FormReport700", "-----", 0, QApplication::UnicodeUTF8));
        label_num->setText(QApplication::translate("FormReport700", "-----", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_item->verticalHeaderItem(0);
        ___qtablewidgetitem15->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_item->verticalHeaderItem(1);
        ___qtablewidgetitem16->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget_item->verticalHeaderItem(2);
        ___qtablewidgetitem17->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget_item->verticalHeaderItem(3);
        ___qtablewidgetitem18->setText(QApplication::translate("FormReport700", "\346\226\260\345\273\272\350\241\214", 0, QApplication::UnicodeUTF8));
        label_print->setText(QApplication::translate("FormReport700", "-----", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FormReport700: public Ui_FormReport700 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMREPORT700_H
