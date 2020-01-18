/********************************************************************************
** Form generated from reading UI file 'formsale700.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMSALE700_H
#define UI_FORMSALE700_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Formsale700
{
public:
    QLabel *label_F4;
    QLabel *label_stable;
    QLabel *label_weight;
    QLabel *label_mode;
    QLabel *label_wifi;
    QLabel *label_ACDC;
    QLabel *label_plu;
    QLabel *label_tare;
    QLabel *label_price;
    QLabel *label_zero;
    QLabel *label_total;
    QLabel *label_F2;
    QLabel *label_describe;
    QLabel *label_QTime;
    QLabel *label_F3;
    QLabel *label_F1;
    QLabel *label_net;
    QLabel *label_ver;
    QLabel *label_Name;
    QLabel *label_cas;
    QLabel *label_Externalweb;
    QLabel *label_update;

    void setupUi(QWidget *Formsale700)
    {
        if (Formsale700->objectName().isEmpty())
            Formsale700->setObjectName(QString::fromUtf8("Formsale700"));
        Formsale700->resize(800, 480);
        Formsale700->setMinimumSize(QSize(800, 480));
        Formsale700->setMaximumSize(QSize(800, 480));
        Formsale700->setStyleSheet(QString::fromUtf8("#Formsale700{border-image: url(:/image/sale.png);}"));
        label_F4 = new QLabel(Formsale700);
        label_F4->setObjectName(QString::fromUtf8("label_F4"));
        label_F4->setGeometry(QRect(350, 5, 41, 21));
        label_stable = new QLabel(Formsale700);
        label_stable->setObjectName(QString::fromUtf8("label_stable"));
        label_stable->setGeometry(QRect(20, 340, 21, 17));
        label_weight = new QLabel(Formsale700);
        label_weight->setObjectName(QString::fromUtf8("label_weight"));
        label_weight->setGeometry(QRect(60, 340, 321, 81));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\226\207\346\263\211\351\251\277\345\276\256\347\261\263\351\273\221"));
        font.setPointSize(36);
        font.setBold(false);
        font.setWeight(50);
        label_weight->setFont(font);
        label_weight->setAutoFillBackground(false);
        label_weight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_mode = new QLabel(Formsale700);
        label_mode->setObjectName(QString::fromUtf8("label_mode"));
        label_mode->setGeometry(QRect(409, 4, 51, 21));
        label_mode->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 16pt \"AR PL UKai CN\";"));
        label_wifi = new QLabel(Formsale700);
        label_wifi->setObjectName(QString::fromUtf8("label_wifi"));
        label_wifi->setGeometry(QRect(54, 5, 31, 21));
        label_wifi->setStyleSheet(QString::fromUtf8("border-image: url(:/image/wlant.png);"));
        label_ACDC = new QLabel(Formsale700);
        label_ACDC->setObjectName(QString::fromUtf8("label_ACDC"));
        label_ACDC->setGeometry(QRect(0, 3, 51, 25));
        label_ACDC->setStyleSheet(QString::fromUtf8("border-image: url(:/image/dc2.png);"));
        label_plu = new QLabel(Formsale700);
        label_plu->setObjectName(QString::fromUtf8("label_plu"));
        label_plu->setGeometry(QRect(0, 43, 791, 71));
        label_plu->setLayoutDirection(Qt::RightToLeft);
        label_plu->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 36pt \"\346\226\207\346\263\211\351\251\277\346\255\243\351\273\221\";"));
        label_tare = new QLabel(Formsale700);
        label_tare->setObjectName(QString::fromUtf8("label_tare"));
        label_tare->setGeometry(QRect(40, 180, 341, 71));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\226\207\346\263\211\351\251\277\346\255\243\351\273\221"));
        font1.setPointSize(36);
        font1.setBold(false);
        font1.setWeight(50);
        label_tare->setFont(font1);
        label_tare->setAutoFillBackground(false);
        label_tare->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_price = new QLabel(Formsale700);
        label_price->setObjectName(QString::fromUtf8("label_price"));
        label_price->setGeometry(QRect(450, 180, 331, 71));
        label_price->setFont(font);
        label_price->setAutoFillBackground(false);
        label_price->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_zero = new QLabel(Formsale700);
        label_zero->setObjectName(QString::fromUtf8("label_zero"));
        label_zero->setGeometry(QRect(20, 400, 21, 17));
        label_total = new QLabel(Formsale700);
        label_total->setObjectName(QString::fromUtf8("label_total"));
        label_total->setGeometry(QRect(440, 340, 341, 81));
        label_total->setFont(font);
        label_total->setAutoFillBackground(false);
        label_total->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_F2 = new QLabel(Formsale700);
        label_F2->setObjectName(QString::fromUtf8("label_F2"));
        label_F2->setGeometry(QRect(216, 5, 41, 21));
        label_describe = new QLabel(Formsale700);
        label_describe->setObjectName(QString::fromUtf8("label_describe"));
        label_describe->setGeometry(QRect(10, 435, 791, 33));
        label_describe->setLayoutDirection(Qt::RightToLeft);
        label_describe->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 20pt \"\346\226\207\346\263\211\351\251\277\346\255\243\351\273\221\";"));
        label_QTime = new QLabel(Formsale700);
        label_QTime->setObjectName(QString::fromUtf8("label_QTime"));
        label_QTime->setGeometry(QRect(530, -1, 271, 31));
        label_QTime->setLayoutDirection(Qt::LeftToRight);
        label_F3 = new QLabel(Formsale700);
        label_F3->setObjectName(QString::fromUtf8("label_F3"));
        label_F3->setGeometry(QRect(280, 5, 41, 21));
        label_F1 = new QLabel(Formsale700);
        label_F1->setObjectName(QString::fromUtf8("label_F1"));
        label_F1->setGeometry(QRect(154, 5, 41, 21));
        label_net = new QLabel(Formsale700);
        label_net->setObjectName(QString::fromUtf8("label_net"));
        label_net->setGeometry(QRect(20, 370, 21, 17));
        label_ver = new QLabel(Formsale700);
        label_ver->setObjectName(QString::fromUtf8("label_ver"));
        label_ver->setGeometry(QRect(462, 6, 61, 20));
        label_Name = new QLabel(Formsale700);
        label_Name->setObjectName(QString::fromUtf8("label_Name"));
        label_Name->setGeometry(QRect(467, 45, 321, 31));
        label_Name->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 127);"));
        label_cas = new QLabel(Formsale700);
        label_cas->setObjectName(QString::fromUtf8("label_cas"));
        label_cas->setGeometry(QRect(467, 87, 321, 31));
        label_cas->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        label_Externalweb = new QLabel(Formsale700);
        label_Externalweb->setObjectName(QString::fromUtf8("label_Externalweb"));
        label_Externalweb->setGeometry(QRect(88, 5, 31, 21));
        label_Externalweb->setStyleSheet(QString::fromUtf8("border-image: url(:/image/On-line-wai.png);"));
        label_update = new QLabel(Formsale700);
        label_update->setObjectName(QString::fromUtf8("label_update"));
        label_update->setGeometry(QRect(220, 230, 371, 81));
        QFont font2;
        font2.setPointSize(72);
        label_update->setFont(font2);
        label_update->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        label_update->setAlignment(Qt::AlignCenter);

        retranslateUi(Formsale700);

        QMetaObject::connectSlotsByName(Formsale700);
    } // setupUi

    void retranslateUi(QWidget *Formsale700)
    {
        Formsale700->setWindowTitle(QApplication::translate("Formsale700", "Form", 0, QApplication::UnicodeUTF8));
        label_F4->setText(QApplication::translate("Formsale700", "F4", 0, QApplication::UnicodeUTF8));
        label_stable->setText(QApplication::translate("Formsale700", "----", 0, QApplication::UnicodeUTF8));
        label_weight->setText(QApplication::translate("Formsale700", "000000", 0, QApplication::UnicodeUTF8));
        label_mode->setText(QApplication::translate("Formsale700", "----", 0, QApplication::UnicodeUTF8));
        label_ACDC->setText(QString());
        label_plu->setText(QApplication::translate("Formsale700", "--------------", 0, QApplication::UnicodeUTF8));
        label_tare->setText(QApplication::translate("Formsale700", "000000", 0, QApplication::UnicodeUTF8));
        label_price->setText(QApplication::translate("Formsale700", "000000", 0, QApplication::UnicodeUTF8));
        label_zero->setText(QApplication::translate("Formsale700", "----", 0, QApplication::UnicodeUTF8));
        label_total->setText(QApplication::translate("Formsale700", "000000", 0, QApplication::UnicodeUTF8));
        label_F2->setText(QApplication::translate("Formsale700", "F2", 0, QApplication::UnicodeUTF8));
        label_describe->setText(QApplication::translate("Formsale700", "--------------", 0, QApplication::UnicodeUTF8));
        label_QTime->setText(QApplication::translate("Formsale700", "2020-01-01 12:30:30", 0, QApplication::UnicodeUTF8));
        label_F3->setText(QApplication::translate("Formsale700", "F3", 0, QApplication::UnicodeUTF8));
        label_F1->setText(QApplication::translate("Formsale700", "F1", 0, QApplication::UnicodeUTF8));
        label_net->setText(QApplication::translate("Formsale700", "----", 0, QApplication::UnicodeUTF8));
        label_ver->setText(QApplication::translate("Formsale700", "----", 0, QApplication::UnicodeUTF8));
        label_Name->setText(QApplication::translate("Formsale700", "\345\225\206\346\210\267:\346\265\213\350\257\225\345\225\206\346\210\267", 0, QApplication::UnicodeUTF8));
        label_cas->setText(QApplication::translate("Formsale700", "\345\270\202\345\234\272\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        label_Externalweb->setText(QString());
        label_update->setText(QApplication::translate("Formsale700", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Formsale700: public Ui_Formsale700 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMSALE700_H
