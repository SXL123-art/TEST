#ifndef ADDSALEDIALOG_H
#define ADDSALEDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QDesktopWidget>
#include "../public/typeTabUi.h"
namespace Ui {
    class AddSaleDialog;
}

class AddSaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSaleDialog(QWidget *parent = 0);
    ~AddSaleDialog();
    void updateAddSaleUi(stAddSaleQT*  m_pAddSaleUi);

private:
    Ui::AddSaleDialog *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ADDSALEDIALOG_H
