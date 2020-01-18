#include "formcalibration700.h"

#include "ui_formcalibration700.h"

FormCalibration700::FormCalibration700(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCalibration700)
{
    ui->setupUi(this);
    calMenu = new strMenu[9];
    calMenu[0].menu = "1.满量程";
    calMenu[1].menu = "2.标定量程";
    calMenu[2].menu = "3.e";
    calMenu[3].menu = "4.e1";
    calMenu[4].menu = "5.本地重力g";
    calMenu[5].menu = "6.目的地重力g";
    calMenu[6].menu = "7.空载内码";
    calMenu[7].menu = "8.满载内码";
    calMenu[8].menu = "9.是否双量程";

    for(int i = 0;i<=CAL_SIZE;i++)
    {
        for(int j = 0;j<3;j++)
        {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem());
        }
    }
    ui->tableWidget->verticalHeader()->setVisible(false);       //隐藏列表头
    ui->tableWidget->horizontalHeader()->setVisible(false);     //隐藏行表头
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    //隐藏列拉动条
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //隐藏行拉动条
    ui->tableWidget->setStyleSheet("background-color:black;"
                                      "font:43pt  \"simhei\";"
                                    "gridline-color:yellow;"
                                    "color:white;"
                                      );
    ui->tableWidget->setSpan(0, 0, 1, 3);
    ui->tableWidget->item(0,0)->setTextAlignment(Qt::AlignHCenter |  Qt::AlignVCenter);
    //ui->tableWidget->setColumnWidth(0,120);
    ui->tableWidget->item(0,0)->setText("一键标定");
    ui->tableWidget->item(0,0)->setTextColor(Qt::red);

    for(int i =1; i<=CAL_SIZE;i++)
    {
        ui->tableWidget->item(i,0)->setText(calMenu[i-1].menu);
    }
}

FormCalibration700::~FormCalibration700()
{
    delete ui;
}

void FormCalibration700::updateCalibrationUi(stTabCalibrationUi*  m_TabCalibrationUi)
{
    //qDebug() << "updateCalibrationUi focus_parent :" <<m_TabCalibrationUi->focus_parent;
    for(int i =1; i<=CAL_SIZE ;i++)
    {
        ui->tableWidget->item(i,1)->setText("");
        ui->tableWidget->item(i,2)->setText("");
        ui->tableWidget->item(i,0)->setTextColor(Qt::white);
    }
     ui->tableWidget->item(m_TabCalibrationUi->focus_parent,0)->setTextColor(Qt::red);

     ui->tableWidget->item(m_TabCalibrationUi->focus_parent,1)->setText(m_TabCalibrationUi->menuData);
     ui->tableWidget->item(m_TabCalibrationUi->focus_parent,1)->setTextColor(Qt::yellow);

     ui->tableWidget->item(m_TabCalibrationUi->focus_parent,2)->setTextColor(Qt::red);
     ui->tableWidget->item(m_TabCalibrationUi->focus_parent,2)->setText(m_TabCalibrationUi->Statusbar);

    /*for(int i =1; i<=CAL_SIZE;i++)
    {
        ui->tableWidget->item(i,0)->setText(calMenu[i].menu);
    }*/
}
void FormCalibration700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

