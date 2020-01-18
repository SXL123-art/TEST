
#include "formgoodskey700.h"

#include "ui_formgoodskey700.h"
FormGoodsKey700::FormGoodsKey700(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormGoodsKey700)
{
    ui->setupUi(this);
    //商品快捷键表5行12列
    for(int i = 0;i<5;i++)
    {
        for(int j = 0;j<12;j++)  
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem());
        }
    }

    ui->tableWidget->verticalHeader()->setVisible(false);       //隐藏列表头
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    //隐藏列拉动条
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //隐藏行拉动条
    ui->tableWidget->setStyleSheet("background:transparent;"
                                   "border-style:outset;"
                                      "font:30pt  \"simhei\";"
                                    "gridline-color:yellow;"
                                    "color:black;"
                                      );
    this->ui->lineEdit_goodsCode->setStyleSheet("QLineEdit {font:bold 40pt \"simhei\";}");
    this->ui->lineEdit_keys->setStyleSheet("QLineEdit {font:bold 40pt \"simhei\";}");
    this->ui->lineEdit_goodsCode->setPlaceholderText("在此输入商品编码");
    this->ui->lineEdit_keys->setPlaceholderText("在键盘上选择快捷键");

    this->ui->label_msg->setStyleSheet("QLabel {color:red;font:bold 40pt \"simhei\";}");
    //this->ui->label_print->setStyleSheet("QLabel {color:red;font:bold 40pt \"simhei\";}");
    this->ui->label_print->setStyleSheet("QLabel {color:black;font:40pt \"simhei\";}");
}

FormGoodsKey700::~FormGoodsKey700()
{
    delete ui;
}

void FormGoodsKey700::updateGoodsKeyUi(stGoodsKeyQT*  m_TabGoodsKeyUi)
{
    if(m_TabGoodsKeyUi->cmd == false)
    {
        this->ui->lineEdit_goodsCode->setStyleSheet("QLineEdit {font:bold 50pt \"simhei\";border-width: 2px;border-style: solid;border-color: red;}");
        this->ui->lineEdit_keys->setStyleSheet("QLineEdit {font:bold 50pt \"simhei\";}");
        ui->lineEdit_goodsCode->setText(m_TabGoodsKeyUi->goodsCode);
        ui->lineEdit_keys->setText(m_TabGoodsKeyUi->goodsKey);
    }
    else
    {
        this->ui->lineEdit_keys->setStyleSheet("QLineEdit {font:bold 50pt \"simhei\";border-width: 2px;border-style: solid;border-color: red;}");
        this->ui->lineEdit_goodsCode->setStyleSheet("QLineEdit {font:bold 50pt \"simhei\";}");
        ui->lineEdit_goodsCode->setText(m_TabGoodsKeyUi->goodsCode);
         ui->lineEdit_keys->setText(m_TabGoodsKeyUi->goodsKey);
    }
    if(m_TabGoodsKeyUi->index>0 && m_TabGoodsKeyUi->index<=60)
    {
        unsigned char row = (m_TabGoodsKeyUi->index-1)/12;
        unsigned char col = (m_TabGoodsKeyUi->index-1)%12;
        ui->tableWidget->item(row,col)->setText(m_TabGoodsKeyUi->goodsName);
    }
    ui->label_msg->setText(m_TabGoodsKeyUi->message);
}

void FormGoodsKey700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

