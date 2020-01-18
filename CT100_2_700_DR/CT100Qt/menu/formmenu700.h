#ifndef FORMMENU700_H
#define FORMMENU700_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include "../public/typeTabUi.h"
#include "../public/QtMenu.h"

namespace Ui {
    class Formmenu700;
}

class Formmenu700 : public QWidget
{
    Q_OBJECT

public:
    explicit Formmenu700(QWidget *parent = 0);
    ~Formmenu700();
    void updateMenuUi(stTabMenuUi*  m_TabMenuUi);

private:
    Ui::Formmenu700 *ui;
    strMenu *mainMenu;
   strMenu *setNetwork;
   strMenu *setUserInfo;
   strMenu *setCustomer;
   strMenu *setSystem;
   strMenu *setData;
   strMenu *setUserSwitch;
   strMenu *setSystemSwitch;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // FORMMENU700_H
