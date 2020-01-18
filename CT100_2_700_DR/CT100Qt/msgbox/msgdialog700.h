#ifndef MSGDIALOG700_H
#define MSGDIALOG700_H

#include <QDialog>
#include <QPainter>
#include <QDesktopWidget>
#include "../public/typeTabUi.h"
namespace Ui {
    class msgDialog700;
}

class msgDialog700 : public QDialog
{
    Q_OBJECT

public:
    explicit msgDialog700(QWidget *parent = 0);
    ~msgDialog700();
    void updateMsgUi(stMessageQT*  m_pMsgnUi);

private:
    Ui::msgDialog700 *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // MSGDIALOG700_H
