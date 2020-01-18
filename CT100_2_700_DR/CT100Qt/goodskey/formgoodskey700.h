#ifndef FORMGOODSKEY700_H
#define FORMGOODSKEY700_H

#include <QWidget>
#include <QPainter>
#include <QDesktopWidget>
#include "../public/typeTabUi.h"

namespace Ui {
    class FormGoodsKey700;
}

class FormGoodsKey700 : public QWidget
{
    Q_OBJECT

public:
    explicit FormGoodsKey700(QWidget *parent = 0);
    ~FormGoodsKey700();
    void updateGoodsKeyUi(stGoodsKeyQT*  m_TabGoodsKeyUi);

private:
    Ui::FormGoodsKey700 *ui;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // FORMGOODSKEY700_H
