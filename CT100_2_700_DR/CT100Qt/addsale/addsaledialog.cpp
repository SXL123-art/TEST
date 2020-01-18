#include "addsaledialog.h"
#include "ui_addsaledialog.h"

AddSaleDialog::AddSaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSaleDialog)
{
    ui->setupUi(this);
    //界面设置宽540 高280
   this->setWindowFlags(Qt::FramelessWindowHint);   // 去掉标题栏
   //消息框居中显示
   this->move ((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);
   //设置背景图片
   this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
   this->ui->label_title->setStyleSheet("QLabel {color:red;font:bold 40pt \"simhei\";}");
   QString sheet = "QLabel {font: 50pt \"simhei\";}";
   QString sheetLabel = "QLabel {font:bold 50pt \"simhei\";}";

   this->ui->label_name->setStyleSheet(sheet);
   this->ui->label->setStyleSheet(sheetLabel);
   this->ui->label_2->setStyleSheet(sheetLabel);
   this->ui->label_3->setStyleSheet(sheetLabel);
   this->ui->label_4->setStyleSheet(sheetLabel);
   this->ui->label_weight->setStyleSheet(sheet);
   this->ui->label_price->setStyleSheet(sheet);
   this->ui->label_totel->setStyleSheet(sheet);

   this->ui->label_msg->setStyleSheet("QLabel {color:blue;font:bold 50pt \"simhei\";}");
   this->ui->label_msg->setAlignment(Qt::AlignCenter);

   this->ui->label_discount->setStyleSheet("QLabel {color:red;font:bold 50pt \"simhei\";}");
   this->ui->label_discount->setAlignment(Qt::AlignCenter);
}

AddSaleDialog::~AddSaleDialog()
{
    delete ui;
}
void AddSaleDialog::updateAddSaleUi(stAddSaleQT*  m_pAddSaleUi)
{
    switch(m_pAddSaleUi->cmd)
    {
        case MSG_MESSAGE_REQ:
            ui->label_title->setText(m_pAddSaleUi->title);
            ui->label_name->setText(m_pAddSaleUi->name);
            ui->label_weight->setText(m_pAddSaleUi->weight);
            ui->label_price->setText(m_pAddSaleUi->price);
            ui->label_totel->setText(m_pAddSaleUi->total);
            ui->label_msg->setText(m_pAddSaleUi->msg);
            ui->label_discount->setText(m_pAddSaleUi->discount);
            this->show();
            break;
        case MSG_MESSAGE_OK:
            this->hide();
            break;
        default:
            break;
    }
}

void AddSaleDialog::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
