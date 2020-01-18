/********************************************************************************
** Form generated from reading UI file 'inputdialog700.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTDIALOG700_H
#define UI_INPUTDIALOG700_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_inputDialog700
{
public:
    QLabel *label_title;
    QLineEdit *lineEdit_input;

    void setupUi(QDialog *inputDialog700)
    {
        if (inputDialog700->objectName().isEmpty())
            inputDialog700->setObjectName(QString::fromUtf8("inputDialog700"));
        inputDialog700->resize(540, 280);
        inputDialog700->setMinimumSize(QSize(540, 280));
        inputDialog700->setMaximumSize(QSize(540, 280));
        inputDialog700->setStyleSheet(QString::fromUtf8("#inputDialog700{border-image: url(:/image/dialog.png);}"));
        label_title = new QLabel(inputDialog700);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setGeometry(QRect(10, 10, 421, 41));
        lineEdit_input = new QLineEdit(inputDialog700);
        lineEdit_input->setObjectName(QString::fromUtf8("lineEdit_input"));
        lineEdit_input->setGeometry(QRect(20, 110, 471, 41));

        retranslateUi(inputDialog700);

        QMetaObject::connectSlotsByName(inputDialog700);
    } // setupUi

    void retranslateUi(QDialog *inputDialog700)
    {
        inputDialog700->setWindowTitle(QApplication::translate("inputDialog700", "Dialog", 0, QApplication::UnicodeUTF8));
        label_title->setText(QApplication::translate("inputDialog700", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class inputDialog700: public Ui_inputDialog700 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTDIALOG700_H
