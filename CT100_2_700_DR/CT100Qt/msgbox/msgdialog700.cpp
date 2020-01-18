#include "msgdialog700.h"
#include "ui_msgdialog700.h"

msgDialog700::msgDialog700(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::msgDialog700)
{
    ui->setupUi(this);
     //界面设置宽540 高280
    this->setWindowFlags(Qt::FramelessWindowHint);   // 去掉标题栏
    //消息框居中显示
    this->move ((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);
    //设置背景图片
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    this->ui->label_title->setStyleSheet("QLabel {color:red;font:bold 60pt \"simhei\";}");

    this->ui->label_msg->setStyleSheet("QLabel {font:bold 60pt \"simhei\";}");
   // this->ui->label_msg->adjustSize();  // 自适应text的大小
    this->ui->label_msg->setAlignment(Qt::AlignCenter);
  //  this->ui->label_msg->setGeometry(QRect(30, 100, 481, 20*4));  //四倍行距
  //  this->ui->label_msg->setWordWrap(true);
 //  this->ui->label_msg->setAlignment(Qt::AlignTop);
}

msgDialog700::~msgDialog700()
{
    delete ui;
}

void msgDialog700::updateMsgUi(stMessageQT*  m_pMsgnUi)
{
    switch(m_pMsgnUi->cmd)
    {
        case MSG_MESSAGE_REQ:
            ui->label_title->setText(m_pMsgnUi->title);
            ui->label_msg->setText(m_pMsgnUi->message);
            this->show();
            break;
        case MSG_MESSAGE_OK:
            this->hide();
            break;
        default:
            break;
    }
}

void msgDialog700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
