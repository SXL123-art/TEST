#ifndef FORMCALIBRATION700_H
#define FORMCALIBRATION700_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include "../public/typeTabUi.h"
#include "../public/QtMenu.h"


namespace Ui {
    class FormCalibration700;
}

class FormCalibration700 : public QWidget
{
    Q_OBJECT

public:
    explicit FormCalibration700(QWidget *parent = 0);
    ~FormCalibration700();
    void updateCalibrationUi(stTabCalibrationUi*  m_TabCalibrationUi);

private:
    Ui::FormCalibration700 *ui;
    strMenu *calMenu;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // FORMCALIBRATION700_H
