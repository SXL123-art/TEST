#include "loginform700.h"
#include "ui_loginform700.h"

LoginForm700::LoginForm700(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm700)
{
    ui->setupUi(this);
    //设置背景图片
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    ui->lineEdit_psw->setEchoMode(QLineEdit::Password);            //  输入时显示原点

    this->ui->label_1->setStyleSheet("QLabel {color:white;font:bold 60pt \"simhei\";}");
    this->ui->label_2->setStyleSheet("QLabel {color:white;font:bold 60pt \"simhei\";}");

    QString LabelSheet1 = "QLabel {color:red;font:bold 120pt \"DS-DIGIB\";}";
    QString LabelSheet2 = "QLabel {color:red;font: 40pt \"simhei\";}";

    this->ui->label_tare->setStyleSheet(LabelSheet1);
    this->ui->label_weight->setStyleSheet(LabelSheet1);
    this->ui->label_tare->setAlignment(Qt::AlignRight);
    this->ui->label_weight->setAlignment(Qt::AlignRight);

    this->ui->lineEdit_user->setStyleSheet("QLineEdit {font:bold 60pt \"simhei\";}");
    this->ui->lineEdit_psw->setStyleSheet("QLineEdit {font:bold 60pt \"simhei\";}");
    this->ui->lineEdit_user->setPlaceholderText("在此输入登录用户名");
    this->ui->lineEdit_psw->setPlaceholderText("在此输入登录用户名密码");

    this->ui->label_hdv->setStyleSheet(LabelSheet2);
    this->ui->label_swv->setStyleSheet(LabelSheet2);

    this->ui->label_msg->setStyleSheet("QLabel {font: 60pt \"simhei\";}");
}

LoginForm700::~LoginForm700()
{
    delete ui;
}
void LoginForm700::updateLoginUi(stTabLoginUi*  m_TabLoginUi)
{
    if(m_TabLoginUi->select == false)
    {
        this->ui->lineEdit_user->setStyleSheet("QLineEdit {font:bold 50pt \"simhei\";border-width: 2px;border-style: solid;border-color: red;}");
        this->ui->lineEdit_psw->setStyleSheet("QLineEdit {font:bold 50pt \"simhei\";}");
    }
    else
    {
        this->ui->lineEdit_psw->setStyleSheet("QLineEdit {font:bold 50pt \"simhei\";border-width: 2px;border-style: solid;border-color: red;}");
        this->ui->lineEdit_user->setStyleSheet("QLineEdit {font:bold 50pt \"simhei\";}");
    }
    if(atof(m_TabLoginUi->weight) >= m_TabLoginUi->m_nMaxWeight)
    {
        ui->label_weight->setText("---------");
        ui->label_tare->setText("---------");
    }
    else
    {
        ui->label_tare->setText(m_TabLoginUi->tare);
        ui->label_weight->setText(m_TabLoginUi->weight);
    }
    ui->label_hdv->setText(tr(m_TabLoginUi->hdversion));
    ui->label_swv->setText(tr(m_TabLoginUi->swversion));
    ui->lineEdit_user->setText(m_TabLoginUi->user);
    ui->lineEdit_psw->setText(m_TabLoginUi->psw);
    ui->label_msg->setText(m_TabLoginUi->message);
}

void LoginForm700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
