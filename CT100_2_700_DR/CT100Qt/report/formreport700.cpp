#include "formreport700.h"
#include "ui_formreport700.h"

FormReport700::FormReport700(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormReport700)
{
    ui->setupUi(this);
    //明细表11行4列 前10行数据，11行合计
    for(int i = 0;i<REPORT_SIZE;i++)
    {
        for(int j = 0;j<4;j++)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem());
        }
    }
    //类型表4行1列
    for(int i = 0;i<4;i++)
    {
        for(int j = 0;j<1;j++)
        {
            ui->tableWidget_item->setItem(i,j,new QTableWidgetItem());
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

    ui->tableWidget->setColumnWidth(0,220);
    ui->tableWidget->setColumnWidth(1,125);
    ui->tableWidget->setColumnWidth(2,125);
    ui->tableWidget->setColumnWidth(3,125);

    headerList<<"商品名称"<<"重量/件数"<<"总价"<<"支付方式";
    headerTotle <<"支付方式"<<"合计笔数"<<"合计总金额"<<"";

    ui->tableWidget_item->horizontalHeader()->setVisible(false);     //隐藏行表头
    ui->tableWidget_item->verticalHeader()->setVisible(false);       //隐藏列表头
    ui->tableWidget_item->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidget_item->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    //隐藏列拉动条
    ui->tableWidget_item->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //隐藏行拉动条
    ui->tableWidget_item->setStyleSheet("background:transparent;"
                                   "border-style:outset;"
                                      "font:50pt  \"simhei\";"
                                    "gridline-color:yellow;"
                                    "color:white;"
                                      );
    ui->tableWidget_item->setColumnWidth(0,200);
    ui->tableWidget_item->item(0,0)->setText("1.日报表");
    ui->tableWidget_item->item(0,1)->setText("2.报表明细");
    ui->tableWidget_item->item(0,2)->setText("3.已上传明细");
    ui->tableWidget_item->item(0,3)->setText("4.未上传明细");

    this->ui->label_title->setStyleSheet("QLabel {color:yellow; font:30pt \"simhei\";}");
    this->ui->label_next->setStyleSheet("QLabel {color:black; font:30pt \"simhei\";}");
    this->ui->label_prior->setStyleSheet("QLabel {color:black; font:30pt \"simhei\";}");
    this->ui->label_num->setStyleSheet("QLabel {color:black; font:30pt \"simhei\";}");
    this->ui->label_print->setStyleSheet("QLabel {color:black; font:30pt \"simhei\";}");

    ui->label_next->setAlignment(Qt::AlignLeft);
    ui->label_prior->setAlignment(Qt::AlignLeft);
    ui->label_num->setAlignment(Qt::AlignLeft);
    ui->label_print->setAlignment(Qt::AlignLeft);
}

FormReport700::~FormReport700()
{
    delete ui;
}

void FormReport700::updateReportUi(stReportQT*  m_TabReportUi)
{
    for(int i = 0;i<REPORT_SIZE;i++)
    {
        ui->tableWidget->item(i,0)->setText("");
        ui->tableWidget->item(i,1)->setText("");
        ui->tableWidget->item(i,2)->setText("");
        ui->tableWidget->item(i,3)->setText("");
    }
    for(int i = 0;i<4;i++)
    {
        ui->tableWidget_item->item(0,i)->setTextColor(Qt::white);
    }

    if(m_TabReportUi->index == 0)
    {
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<""<<""<<""<<"");
        this->ui->label_print->setText("");
       return ;

    }
    else if(m_TabReportUi->index == 1)
    {
        ui->tableWidget->setHorizontalHeaderLabels(headerTotle);
        ui->tableWidget_item->item(0,m_TabReportUi->index-1)->setTextColor(Qt::red);
        this->ui->label_print->setText("打印报表");
    }
    else if(m_TabReportUi->index == 2)
    {
        ui->tableWidget->setHorizontalHeaderLabels(headerList);
        ui->tableWidget_item->item(0,m_TabReportUi->index-1)->setTextColor(Qt::red);
        this->ui->label_print->setText("打印本页");
    }
    else if(m_TabReportUi->index == 3)
    {
        ui->tableWidget->setHorizontalHeaderLabels(headerList);
        ui->tableWidget_item->item(0,m_TabReportUi->index-1)->setTextColor(Qt::red);
        this->ui->label_print->setText("打印本页");
    }
    else if(m_TabReportUi->index == 4)
    {
        ui->tableWidget->setHorizontalHeaderLabels(headerList);
        ui->tableWidget_item->item(0,m_TabReportUi->index-1)->setTextColor(Qt::red);
        this->ui->label_print->setText("打印本页");
    }

    if(m_TabReportUi->totalNum - m_TabReportUi->curNum > 0)
    {
        this->ui->label_next->setText("下一页");
    }
    else
    {
        this->ui->label_next->setText("");
    }
    if(m_TabReportUi->curNum>REPORT_SIZE)
    {
        this->ui->label_prior->setText("上一页");
    }
    else
    {
        this->ui->label_prior->setText("");
    }
    QString StrNum = QString("%1/%2").arg(m_TabReportUi->curNum).arg(m_TabReportUi->totalNum);
    this->ui->label_num->setText(StrNum);

    for(int i = 0;i<REPORT_SIZE;i++)
    {
        ui->tableWidget->item(i,0)->setText(m_TabReportUi->table[i].plu);
        ui->tableWidget->item(i,1)->setText(m_TabReportUi->table[i].weight);
        ui->tableWidget->item(i,2)->setText(m_TabReportUi->table[i].total);

        //qDebug() << "FormReport700--payMode----------------" <<m_TabReportUi->table[i].payMode;
        if(atoi(m_TabReportUi->table[i].payMode) == 1)
        {
            ui->tableWidget->item(i,3)->setText("现金支付");
        }
        else if(atoi(m_TabReportUi->table[i].payMode) == 2)
        {
            ui->tableWidget->item(i,3)->setText("银联卡支付");
        }
        else if(atoi(m_TabReportUi->table[i].payMode) == 3)
        {
            ui->tableWidget->item(i,3)->setText("支付宝支付");
        }
        else if(atoi(m_TabReportUi->table[i].payMode) == 4)
        {
            ui->tableWidget->item(i,3)->setText("微信支付");
        }
        else if(atoi(m_TabReportUi->table[i].payMode) == 5)
        {
            ui->tableWidget->item(i,3)->setText("会员卡支付");
        }
        else if(atoi(m_TabReportUi->table[i].payMode) == 6)
        {
            ui->tableWidget->item(i,3)->setText("市民卡支付");
        }
        else
        {
            ui->tableWidget->item(i,3)->setText("");
        }
    }
}
void FormReport700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
