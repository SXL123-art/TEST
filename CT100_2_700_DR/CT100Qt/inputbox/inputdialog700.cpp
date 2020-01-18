#include "inputdialog700.h"
#include "ui_inputdialog700.h"

inputDialog700::inputDialog700(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputDialog700)
{
    ui->setupUi(this);
    //界面设置宽540 高280
    this->setWindowFlags(Qt::FramelessWindowHint);   // 去掉标题栏
    //消息框居中显示
    this->move ((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);
    //设置背景图片
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.

    this->ui->label_title->setStyleSheet("QLabel {color:red;font:bold 40pt \"simhei\";}");

    this->ui->lineEdit_input->setStyleSheet("QLineEdit {font:bold 40pt \"simhei\";}");
    ui->lineEdit_input->setEchoMode(QLineEdit::Password);
    ui->lineEdit_input->setPlaceholderText("在此输入菜单密码");
}

inputDialog700::~inputDialog700()
{
    delete ui;
}

void inputDialog700::updateInputMsgUi(stInputQT*  m_pMsgnUi)
{
    switch(m_pMsgnUi->cmd)
    {
        case MSG_MESSAGE_REQ:
            this->ui->label_title->setText(m_pMsgnUi->message);
            this->ui->lineEdit_input->setText(m_pMsgnUi->input);
            this->show();
            break;
        case MSG_MESSAGE_OK:
            this->hide();
            break;
        default:
            break;
     }
}

void inputDialog700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
