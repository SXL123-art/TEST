#ifndef FORMPAY700_H
#define FORMPAY700_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include "../public/typeTabUi.h"

namespace Ui {
    class Formpay700;
}

class Formpay700 : public QWidget
{
    Q_OBJECT

public:
    explicit Formpay700(QWidget *parent = 0);
    ~Formpay700();
    void updatePayUi(stTabPayUi*  m_TabPayUi);

private:
    Ui::Formpay700 *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // FORMPAY700_H
