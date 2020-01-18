#ifndef FORMREPORT700_H
#define FORMREPORT700_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QStringList>
#include "../public/typeTabUi.h"
namespace Ui {
    class FormReport700;
}

class FormReport700 : public QWidget
{
    Q_OBJECT

public:
    explicit FormReport700(QWidget *parent = 0);
    ~FormReport700();
     void updateReportUi(stReportQT*  m_TabReportUi);

private:
    Ui::FormReport700 *ui;
    QStringList headerList;
    QStringList headerTotle;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // FORMREPORT700_H
