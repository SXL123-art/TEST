#ifndef SCREENSAVERDIALOG700_H
#define SCREENSAVERDIALOG700_H

#include <QDialog>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include "../public/typeTabUi.h"
namespace Ui {
    class ScreenSaverDialog700;
}

class ScreenSaverDialog700 : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenSaverDialog700(QWidget *parent = 0);
    ~ScreenSaverDialog700();
    void updateScreenSaverDialog700Ui(stScreenCamQT*  m_TabScreenSaverUi);

private:
    Ui::ScreenSaverDialog700 *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // SCREENSAVERDIALOG700_H
