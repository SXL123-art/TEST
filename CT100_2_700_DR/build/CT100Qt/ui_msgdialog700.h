/********************************************************************************
** Form generated from reading UI file 'msgdialog700.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MSGDIALOG700_H
#define UI_MSGDIALOG700_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_msgDialog700
{
public:
    QLabel *label_title;
    QLabel *label_msg;

    void setupUi(QDialog *msgDialog700)
    {
        if (msgDialog700->objectName().isEmpty())
            msgDialog700->setObjectName(QString::fromUtf8("msgDialog700"));
        msgDialog700->resize(600, 300);
        msgDialog700->setMinimumSize(QSize(600, 300));
        msgDialog700->setMaximumSize(QSize(600, 300));
        msgDialog700->setStyleSheet(QString::fromUtf8("#msgDialog700{border-image: url(:/image/dialog.png);}"));
        label_title = new QLabel(msgDialog700);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setGeometry(QRect(10, 10, 531, 41));
        label_msg = new QLabel(msgDialog700);
        label_msg->setObjectName(QString::fromUtf8("label_msg"));
        label_msg->setGeometry(QRect(30, 100, 541, 81));

        retranslateUi(msgDialog700);

        QMetaObject::connectSlotsByName(msgDialog700);
    } // setupUi

    void retranslateUi(QDialog *msgDialog700)
    {
        msgDialog700->setWindowTitle(QApplication::translate("msgDialog700", "Dialog", 0, QApplication::UnicodeUTF8));
        label_title->setText(QApplication::translate("msgDialog700", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_msg->setText(QApplication::translate("msgDialog700", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class msgDialog700: public Ui_msgDialog700 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MSGDIALOG700_H
