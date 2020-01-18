#ifndef INPUTDIALOG700_H
#define INPUTDIALOG700_H

#include <QDialog>
#include <QPainter>
#include <QDesktopWidget>
#include "../public/typeTabUi.h"

namespace Ui {
    class inputDialog700;
}

class inputDialog700 : public QDialog
{
    Q_OBJECT

public:
    explicit inputDialog700(QWidget *parent = 0);
    ~inputDialog700();
    void updateInputMsgUi(stInputQT*  m_pMsgnUi);

private:
    Ui::inputDialog700 *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // INPUTDIALOG700_H
