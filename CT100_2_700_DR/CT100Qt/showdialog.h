#ifndef SHOWDIALOG_H
#define SHOWDIALOG_H

#include <QTimer>
#include <QDialog>
#include <QDebug>
#include <sys/shm.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include "../public/typeTabUi.h"

#include "./sale/formsale700.h"
#include "./menu/formmenu700.h"
#include "./login/loginform700.h"
#include "./pay/formpay700.h"
#include "./calibration/formcalibration700.h"
#include "./report/formreport700.h"
#include "./goodskey/formgoodskey700.h"
#include "./msgbox/msgdialog700.h"
#include "./inputbox/inputdialog700.h"
#include "./addsale/addsaledialog.h"


namespace Ui {
    class showDialog;
}

class showDialog : public QDialog
{
    Q_OBJECT

public:
    explicit showDialog(int screenWidth,int screenHeight,QWidget *parent = 0);
    ~showDialog();
protected:
    void changeEvent(QEvent *e);
    bool modifyFont();


public:
    void initShm();

private:
    Ui::showDialog *ui;
    int ScreenWidth;
    int ScreenHeight;

public:
    sem_t           *semr;			//有名信号灯 读
    sem_t           *semw;			//有名信号灯 写
    char            *shmAddress;

    Formsale700     *m_pFormSale700;
    LoginForm700    *m_pFormLogin700;
    Formmenu700     *m_pFormMenu700;
    Formpay700      *m_pFormPay700;
    FormCalibration700 *m_pFormCalibration700;
    FormReport700     *m_pFormReport700;
    FormGoodsKey700   *m_pFormGoodsKey700;
    msgDialog700    *m_pMsgDialog700;
    inputDialog700  *m_pInputDialog700;
    AddSaleDialog   *m_PAddSaleDialog;

private slots:
    void DataUpdate();
};

#endif // SHOWDIALOG_H
