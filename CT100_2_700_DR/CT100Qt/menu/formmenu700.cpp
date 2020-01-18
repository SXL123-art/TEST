#include "formmenu700.h"
#include "ui_formmenu700.h"

Formmenu700::Formmenu700(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formmenu700)
{
    ui->setupUi(this);

    //设置背景图片
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.

    mainMenu = new strMenu[7];
    mainMenu[0].menu="1、网络设置";
    mainMenu[1].menu="2、用户参数";
    mainMenu[2].menu="3、商户信息";
    mainMenu[3].menu="4、系统设置";
    mainMenu[4].menu="5、数据管理";
    mainMenu[5].menu="6、用户开关";
    mainMenu[6].menu="7、系统开关";

    //0
    setNetwork = new strMenu[12];
    setNetwork[0].menu="外服务器";
    setNetwork[1].menu="内服务器";
    setNetwork[2].menu="MAC地址";
    setNetwork[3].menu="IP地址";
    setNetwork[4].menu="子网掩码";
    setNetwork[5].menu="网关";
    setNetwork[6].menu="网络模式";
    setNetwork[7].menu="SSID";
    setNetwork[8].menu="无线密码";
    setNetwork[9].menu="加密方式";
    setNetwork[10].menu="DHCP";
    setNetwork[11].menu="DNS";

    setUserInfo = new strMenu[14];
    setUserInfo[0].menu="设备ID";
    setUserInfo[1].menu="设备类型";
    setUserInfo[2].menu="摊位号";
    setUserInfo[3].menu="摊主姓名";
    setUserInfo[4].menu="商户折扣";
    setUserInfo[5].menu="商户版本";
    setUserInfo[6].menu="市场编码";
    setUserInfo[7].menu="市场名称";
    setUserInfo[8].menu="头尾1";
    setUserInfo[9].menu="头尾2";
    setUserInfo[10].menu="头尾3";
    setUserInfo[11].menu="头尾4";
    setUserInfo[12].menu="头尾5";
    setUserInfo[13].menu="头尾6";

    setCustomer = new strMenu[11];
    setCustomer[0].menu="支付宝APPID";
    setCustomer[1].menu="支付宝帐号";
    setCustomer[2].menu="微信APPID";
    setCustomer[3].menu="微信帐号";
    setCustomer[4].menu="微信Key";
    setCustomer[5].menu="其他ID";
    setCustomer[6].menu="其他帐号";
    setCustomer[7].menu="蓝牙名称";
    setCustomer[8].menu="蓝牙密码";
    setCustomer[9].menu="Zigbee网络ID";
    setCustomer[10].menu="Zigbee信道";

    setSystem = new strMenu[5];
    setSystem[0].menu="设置系统时间";
    setSystem[1].menu="超时锁屏(S)";
    setSystem[2].menu="重量小数点位置(2,3)";
    setSystem[3].menu="总价四舍五入(0,1,2)";
    setSystem[4].menu="背光亮度(1-9)";

    setData = new strMenu[7];
    setData[0].menu="清除交易信息";
    setData[1].menu="清除商品信息";
    setData[2].menu="清除批次信息";
    setData[3].menu="清除快捷键";
    setData[4].menu="清除操作员信息";
    setData[5].menu="初始化商品信息";
    setData[6].menu="初始化必要菜单参数";

    setUserSwitch = new strMenu[6];
    setUserSwitch[0].menu="无商品不显示总价";
    setUserSwitch[1].menu="保留上次商品";
    setUserSwitch[2].menu="记录商品单价";
    setUserSwitch[3].menu="强制记录";
    setUserSwitch[4].menu="强制打印";
    setUserSwitch[5].menu="强制记录阈值";

    setSystemSwitch = new strMenu[15];
    setSystemSwitch[0].menu="登录模式";
    setSystemSwitch[1].menu="小票数量";
    setSystemSwitch[2].menu="收银箱";
    setSystemSwitch[3].menu="摄像头设备开关";
    setSystemSwitch[4].menu="RFID设备开关";
    setSystemSwitch[5].menu="蓝牙设备卡关";
    setSystemSwitch[6].menu="Zigbee设备开关";
    setSystemSwitch[7].menu="POS机设备开关";
    setSystemSwitch[8].menu="扫描枪设备开关";
    setSystemSwitch[9].menu="上位机开关";
    setSystemSwitch[10].menu="Web平台开关";
    setSystemSwitch[11].menu="支付宝开关";
    setSystemSwitch[12].menu="微信开关";
    setSystemSwitch[13].menu="其他开关";
    setSystemSwitch[14].menu="调试开关";

    //qDebug() << "---------------------------------------------------------------------------menu";
    for(int i = 0;i<9;i++)
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
    ui->tableWidget->setStyleSheet(
                                    "background-color:black;"
                                     "font:42pt  \"simhei\";"
                                    "gridline-color:yellow;"
                                    "color:white;"
                                      );
    ui->tableWidget->setSpan(0, 0, 1, 3);
    ui->tableWidget->item(0,0)->setTextAlignment(Qt::AlignHCenter |  Qt::AlignVCenter);
    //ui->tableWidget->setColumnWidth(0,120);
    ui->tableWidget->item(0,0)->setText("主菜单");
    ui->tableWidget->item(0,0)->setTextColor(Qt::red);
    //ui->tableWidget->item(0,0)->setFont(QFont("48pt \"simhei\"")); //设置字体
    for(int i =1; i<8;i++)
    {
        ui->tableWidget->item(i,0)->setText(mainMenu[i-1].menu);
    }
    this->ui->label_bar->setStyleSheet("QLabel {color:red; font:48pt \"simhei\";}");
}

Formmenu700::~Formmenu700()
{
    delete ui;
}

void Formmenu700::updateMenuUi(stTabMenuUi*  m_TabMenuUi)
{
    //qDebug() << "updateMenuUi focus_parent " <<m_TabMenuUi->focus_parent<< "  focus_second" << m_TabMenuUi->focus_second;
    this->ui->label_bar->setText(m_TabMenuUi->Statusbar);
    for(int i =1; i<9 ;i++)
    {
        ui->tableWidget->item(i,0)->setTextColor(Qt::white);
        ui->tableWidget->item(i,1)->setTextColor(Qt::white);
    }

    if(1 == m_TabMenuUi->focus_parent)//网络参数
    {
        for(int i =1; i<9 ;i++)
        {
            ui->tableWidget->item(i,2)->setText("");
        }
        ui->tableWidget->item(m_TabMenuUi->focus_parent,0)->setTextColor(Qt::red);
        if(true == m_TabMenuUi->focus_child)
        {
            int j=0;
            if(m_TabMenuUi->focus_second > 8)
            {
                j = m_TabMenuUi->focus_second - 8;
                ui->tableWidget->item(8,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(8,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(8,1)->setTextColor(Qt::red);
            }
            else
            {
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(m_TabMenuUi->focus_second,1)->setTextColor(Qt::red);
            }
            for(int i =1; i<9;i++,j++)
            {
                ui->tableWidget->item(i,1)->setText(setNetwork[j].menu);
            }
        }
        else
        {
            for(int i =1,j=0; i<9;i++,j++)
            {
                ui->tableWidget->item(i,1)->setText(setNetwork[j].menu);
            }
        }
    }
    else if(2 == m_TabMenuUi->focus_parent)
    {
        for(int i =1; i<9 ;i++)
        {
            ui->tableWidget->item(i,2)->setText("");
        }
        ui->tableWidget->item(m_TabMenuUi->focus_parent,0)->setTextColor(Qt::red);
        if(true == m_TabMenuUi->focus_child)
        {
            int j=0;
            if(m_TabMenuUi->focus_second > 8)
            {
                j = m_TabMenuUi->focus_second - 8;
                ui->tableWidget->item(8,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(8,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(8,1)->setTextColor(Qt::red);
            }
            else
            {
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(m_TabMenuUi->focus_second,1)->setTextColor(Qt::red);
            }
            for(int i =1; i<9;i++,j++)
            {
                ui->tableWidget->item(i,1)->setText(setUserInfo[j].menu);
            }
        }
        else
        {
            for(int i =1,j=0; i<9;i++,j++)
            {
                ui->tableWidget->item(i,1)->setText(setUserInfo[j].menu);
            }
        }
    }
    else if(3 == m_TabMenuUi->focus_parent)
    {
        for(int i =1; i<9 ;i++)
        {
            ui->tableWidget->item(i,2)->setText("");
        }
        ui->tableWidget->item(m_TabMenuUi->focus_parent,0)->setTextColor(Qt::red);
        if(true == m_TabMenuUi->focus_child)
        {
            int j=0;
            if(m_TabMenuUi->focus_second > 8)
            {
                j = m_TabMenuUi->focus_second - 8;
                ui->tableWidget->item(8,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(8,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(8,1)->setTextColor(Qt::red);
            }
            else
            {
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(m_TabMenuUi->focus_second,1)->setTextColor(Qt::red);
            }
            for(int i =1; i<9;i++,j++)
            {
                ui->tableWidget->item(i,1)->setText(setCustomer[j].menu);
            }
        }
        else
        {
            for(int i =1,j=0; i<9;i++,j++)
            {
                ui->tableWidget->item(i,1)->setText(setCustomer[j].menu);
            }
        }
    }
    else if(4 == m_TabMenuUi->focus_parent )
    {
        for(int i =1; i<9 ;i++)
        {
            ui->tableWidget->item(i,2)->setText("");
            ui->tableWidget->item(i,1)->setText("");
        }
        ui->tableWidget->item(m_TabMenuUi->focus_parent,0)->setTextColor(Qt::red);
        if(true == m_TabMenuUi->focus_child)
        {
            int j=0;
            if(m_TabMenuUi->focus_second > 8)
            {
                j = m_TabMenuUi->focus_second - 8;
                ui->tableWidget->item(8,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(8,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(8,1)->setTextColor(Qt::red);
            }
            else
            {
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(m_TabMenuUi->focus_second,1)->setTextColor(Qt::red);
            }
            for(int i =1; i<9;i++,j++)
            {
                if(j < MENUSYSTEM_SIZE)
                {
                   ui->tableWidget->item(i,1)->setText(setSystem[j].menu);
                }
                else{
                    ui->tableWidget->item(i,1)->setText("");
                }
            }
        }
        else
        {
            for(int i =1,j=0; i<9;i++,j++)
            {
                if(j < MENUSYSTEM_SIZE)
                {
                   ui->tableWidget->item(i,1)->setText(setSystem[j].menu);
                }
                else{
                    ui->tableWidget->item(i,1)->setText("");
                }

            }
        }
    }
    else if(5 == m_TabMenuUi->focus_parent )
    {
        for(int i =1; i<9 ;i++)
        {
            ui->tableWidget->item(i,2)->setText("");
            ui->tableWidget->item(i,1)->setText("");
        }
        ui->tableWidget->item(m_TabMenuUi->focus_parent,0)->setTextColor(Qt::red);
        if(true == m_TabMenuUi->focus_child)
        {
            int j=0;
            if(m_TabMenuUi->focus_second > 8)
            {
                j = m_TabMenuUi->focus_second - 8;
                ui->tableWidget->item(8,2)->setText("");
                ui->tableWidget->item(8,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(8,1)->setTextColor(Qt::red);
            }
            else
            {
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setText("");
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(m_TabMenuUi->focus_second,1)->setTextColor(Qt::red);
            }
            for(int i =1; i<9;i++,j++)
            {
                if(j < MENUDATA_SIZE)
                {
                   ui->tableWidget->item(i,1)->setText(setData[j].menu);
                }
                else{
                    ui->tableWidget->item(i,1)->setText("");
                }
            }
        }
        else
        {
            for(int i =1,j=0; i<9;i++,j++)
            {
                if(j < MENUDATA_SIZE)
                {
                   ui->tableWidget->item(i,1)->setText(setData[j].menu);
                }
                else{
                    ui->tableWidget->item(i,1)->setText("");
                }
            }
        }
    }
    else if(6 == m_TabMenuUi->focus_parent )
    {
        for(int i =1; i<9 ;i++)
        {
            ui->tableWidget->item(i,2)->setText("");
            ui->tableWidget->item(i,1)->setText("");
        }
        ui->tableWidget->item(m_TabMenuUi->focus_parent,0)->setTextColor(Qt::red);
        if(true == m_TabMenuUi->focus_child)
        {
            int j=0;
            if(m_TabMenuUi->focus_second > 8)
            {
                j = m_TabMenuUi->focus_second - 8;
                ui->tableWidget->item(8,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(8,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(8,1)->setTextColor(Qt::red);
            }
            else
            {
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(m_TabMenuUi->focus_second,1)->setTextColor(Qt::red);
            }
            for(int i =1; i<9;i++,j++)
            {
                if(j == MENUUSERSWITCH_SIZE)
                {
                    break;
                }
                ui->tableWidget->item(i,1)->setText(setUserSwitch[j].menu);
            }
        }
        else
        {
            for(int i =1,j=0; i<9;i++,j++)
            {
                if(j == MENUUSERSWITCH_SIZE)
                {
                    break;
                }
                ui->tableWidget->item(i,1)->setText(setUserSwitch[j].menu);
            }
        }
    }
    else if(7 == m_TabMenuUi->focus_parent )
    {
        for(int i =1; i<9 ;i++)
        {
            ui->tableWidget->item(i,2)->setText("");
        }
        ui->tableWidget->item(m_TabMenuUi->focus_parent,0)->setTextColor(Qt::red);
        if(true == m_TabMenuUi->focus_child)
        {
            int j=0;
            if(m_TabMenuUi->focus_second > 8)
            {
                j = m_TabMenuUi->focus_second - 8;
                ui->tableWidget->item(8,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(8,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(8,1)->setTextColor(Qt::red);
            }
            else
            {
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setText(m_TabMenuUi->menuData);
                ui->tableWidget->item(m_TabMenuUi->focus_second,2)->setTextColor(Qt::yellow);
                ui->tableWidget->item(m_TabMenuUi->focus_second,1)->setTextColor(Qt::red);
            }
            for(int i =1; i<9;i++,j++)
            {
                ui->tableWidget->item(i,1)->setText(setSystemSwitch[j].menu);
            }
        }
        else
        {
            for(int i =1,j=0; i<9;i++,j++)
            {
                ui->tableWidget->item(i,1)->setText(setSystemSwitch[j].menu);
            }
        }
    }
}
void Formmenu700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

