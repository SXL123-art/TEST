#ifndef LOGINFORM700_H
#define LOGINFORM700_H

#include <QWidget>
#include <QPainter>
#include "../public/typeTabUi.h"


namespace Ui {
    class LoginForm700;
}

class LoginForm700 : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm700(QWidget *parent = 0);
    ~LoginForm700();

    void updateLoginUi(stTabLoginUi*  m_TabLoginUi);

private:
    Ui::LoginForm700 *ui;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // LOGINFORM700_H
