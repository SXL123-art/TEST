#include "formpay700.h"
#include "ui_formpay700.h"

Formpay700::Formpay700(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formpay700)
{
    ui->setupUi(this);
    //设置背景图片
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QString LabelSheet_pay = "QLabel {color:red;font:bold 80pt \"simhei\";}";
    ui->label_paymode->setStyleSheet(LabelSheet_pay);
    ui->label_paynum->setStyleSheet(LabelSheet_pay);
    ui->label_paymoney->setStyleSheet(LabelSheet_pay);
    this->ui->label_paymode->setAlignment(Qt::AlignRight);
     this->ui->label_paynum->setAlignment(Qt::AlignRight);
     this->ui->label_paymoney->setAlignment(Qt::AlignRight);

    ui->label_paytit->setStyleSheet("QLabel {color:red;font:50pt \"simhei\";}");
    ui->label_payqr->setScaledContents(true);

}

Formpay700::~Formpay700()
{
    delete ui;
}

void Formpay700::updatePayUi(stTabPayUi*  m_TabPayUi)
{
    //qDebug()<<"Formpay700  updatePayUi" ;

    ui->label_paytit->setText(m_TabPayUi->title);
    ui->label_paymode->setText(m_TabPayUi->payMode);
    ui->label_paynum->setText(m_TabPayUi->saleNum);
    ui->label_paymoney->setText(m_TabPayUi->payMoney);

    if(strlen(m_TabPayUi->payQr)>0)
    {
        QPixmap pmQR("./QRBMP/qrpay.bmp");
        ui->label_payqr->setPixmap((pmQR));
    }
    else
    {
        QPixmap pmMode("./QRBMP/payMode.png");
        ui->label_payqr->setPixmap((pmMode));
    }

}

void Formpay700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

