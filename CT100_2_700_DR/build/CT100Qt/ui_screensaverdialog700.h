/********************************************************************************
** Form generated from reading UI file 'screensaverdialog700.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENSAVERDIALOG700_H
#define UI_SCREENSAVERDIALOG700_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_ScreenSaverDialog700
{
public:
    QLabel *label_log;

    void setupUi(QDialog *ScreenSaverDialog700)
    {
        if (ScreenSaverDialog700->objectName().isEmpty())
            ScreenSaverDialog700->setObjectName(QString::fromUtf8("ScreenSaverDialog700"));
        ScreenSaverDialog700->resize(800, 480);
        ScreenSaverDialog700->setMinimumSize(QSize(800, 480));
        ScreenSaverDialog700->setMaximumSize(QSize(800, 480));
        label_log = new QLabel(ScreenSaverDialog700);
        label_log->setObjectName(QString::fromUtf8("label_log"));
        label_log->setGeometry(QRect(0, 0, 800, 480));
        label_log->setMinimumSize(QSize(800, 480));
        label_log->setMaximumSize(QSize(800, 480));

        retranslateUi(ScreenSaverDialog700);

        QMetaObject::connectSlotsByName(ScreenSaverDialog700);
    } // setupUi

    void retranslateUi(QDialog *ScreenSaverDialog700)
    {
        ScreenSaverDialog700->setWindowTitle(QApplication::translate("ScreenSaverDialog700", "Dialog", 0, QApplication::UnicodeUTF8));
        label_log->setText(QApplication::translate("ScreenSaverDialog700", "caslog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ScreenSaverDialog700: public Ui_ScreenSaverDialog700 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENSAVERDIALOG700_H
