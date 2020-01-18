#include "menuNet.h"


CMenuNet::CMenuNet(CShareInfo *m_pInfo)
{
    m_pMenuShareInfo = m_pInfo;
    m_bSetSure = false;
    focus_second = 1;
}

void CMenuNet::init(void)
{
    QSqlQuery query;
    int dbSize;
    QString sqlStr = QString("select * from table_scalesetting where upper='%1';").arg(MENUNET_UPPER);
    if(MENUNET_SIZE == (dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sqlStr,&query)))
    {
        m_pNewSetting = new m_MenuInfo[dbSize];
        bool ok;
        for(int i=0; i<dbSize;i++)
        {
            query.next();
            m_pNewSetting[i].id = query.record().value(TABLE_SCALESETTING_ID).toString().toInt(&ok,10);
            m_pNewSetting[i].MenuInfo = query.record().value(TABLE_SCALESETTING_VALUE).toString();
        }
    }
    focus_second = 1;
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
    //sprintf(m_pMenuShareInfo->shareInfoMenuUi.menuData,"%s",m_pNewSetting[focus_second-1].MenuInfo.toStdString().c_str());
}

void CMenuNet::Deal()
{
    init();
    printf("--------------------------------------------------------------CMenuNet!\n");
    m_bSetSure = false;
    bool UpdataBar = false;
    unsigned char ch;
    //刷新子菜单
     m_bUpdate = true;
    while(true)
    {
        usleep(50*1000);
        if (SALE_KEY_VALID == ATKey_getKey(&ch))
        {
            m_bUpdate = true;
            ProcessEditStr(ch);
            //解析按键值
            switch(ch)
            {
                case _KEY_MENU:	//返回上一级菜单
                    printf("--------------------------------------------------------------CMenuNet return mainMenu\n");
                    if(m_bSetSure == true)
                    {
                        //重启网络
                        RestartNetwork();
                    }
                    ReturnMainMenu();
                    return;
                 case _KEY_ENTER://确定,保存修改,这里写数据库
                    UpdataBar = true;
                    m_bSetSure = true;
                    Sure();
                    break;
                case _KEY_UP:
                   dec();
                   break;
               case _KEY_DOWN:
                   add();
                   break;
               default:
                       break;
                }
            }
        if(m_bUpdate)
        {
            if(false == UpdataBar)
            {
               updataStatusbar();
            }
            updateMenuShow(m_pMenuShareInfo);
            m_bUpdate = false;
            UpdataBar = false;
        }
    }
}
int CMenuNet::Sure(void)
{
    if(false == m_pMenuShareInfo->m_nMenuFlag && focus_second == 3)
    {
        return 0;
    }
    switch(focus_second)
    {
    case 1:
        m_pMenuShareInfo->m_ShareNetInfo.RemoteServer = menuStr;
        break;
    case 2:
        m_pMenuShareInfo->m_ShareNetInfo.InternalServer = menuStr;
        break;
    case 3:
        m_pMenuShareInfo->m_ShareNetInfo.Mac = menuStr;
        break;
    case 4:
        m_pMenuShareInfo->m_ShareNetInfo.Ip = menuStr;
        break;
    case 5:
        m_pMenuShareInfo->m_ShareNetInfo.Mask = menuStr;
        break;
    case 6:
        m_pMenuShareInfo->m_ShareNetInfo.Gateway = menuStr;
        break;
    case 7:
        m_pMenuShareInfo->m_ShareNetInfo.Net_Mode = menuStr;
        break;
    case 8:
        m_pMenuShareInfo->m_ShareNetInfo.SSID = menuStr;
        break;
    case 9:
        m_pMenuShareInfo->m_ShareNetInfo.PassWord = menuStr;
        break;
    case 10:
        m_pMenuShareInfo->m_ShareNetInfo.Encryption = menuStr;
        break;
    case 11:
        m_pMenuShareInfo->m_ShareNetInfo.Dhcp = menuStr;
        break;
    case 12:
        m_pMenuShareInfo->m_ShareNetInfo.Dns = menuStr;
        break;
    }

    QString sqlStr = QString("update table_scalesetting  set value='%1' where id=%2;").arg(m_pNewSetting[focus_second-1].MenuInfo).arg(m_pNewSetting[focus_second-1].id);
    qDebug()<<sqlStr;
    if(ATdb_Excute_SQL_Statements(sqlStr)<=0)
    {
        qDebug() <<"update db fail-----------" << menuStr;
        SetStatusBar(m_pMenuShareInfo,0,menuStr);
        return 0;
    }
    else{
        SetStatusBar(m_pMenuShareInfo,1,menuStr);
    }
    if(focus_second == 12)
    {
        // 修改DNS
        system("echo -n \" \"> /etc/resolv.conf");
        char Command[64] = {0};
        sprintf(Command, "echo nameserver %s > /etc/resolv.conf", m_pNewSetting[11].MenuInfo.toStdString().c_str());
        system(Command);
    }
}

void CMenuNet::ReturnMainMenu()
{
    menuStr.clear();
    sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
    delete [] m_pNewSetting;
    m_pNewSetting = NULL;
}

void CMenuNet::dec()
{
    if(focus_second>1)
    {
        focus_second--;
    }
    else
    {
        focus_second = focus_second+MENUNET_SIZE-1;
    }
    printf("dec focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}

void CMenuNet::add()
{
    if(focus_second<MENUNET_SIZE)
    {
        focus_second++;
    }
    else{
        focus_second = focus_second-MENUNET_SIZE+1;
    }
    printf("add focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}
void CMenuNet::ProcessEditStr(unsigned char ch)
{
    if(false == m_pMenuShareInfo->m_nMenuFlag && focus_second == 3)
    {
        return;
    }
    char cha;
    if ((ch >= _KEY_PLU1) && (ch <= _KEY_PLU72)
        || (ch >= _KEY_NUM0) && (ch <= _KEY_NUM9)
        || (ch == _KEY_DOT)
        || (ch == _KEY_BACK)
        || (ch == _KEY_SHIFT)
        || (ch == _KEY_CLR))
    {
        if(ch == _KEY_BACK)
        {
            if(m_pNewSetting[focus_second-1].MenuInfo.length()>0)
            {
                m_pNewSetting[focus_second-1].MenuInfo = m_pNewSetting[focus_second-1].MenuInfo.left(m_pNewSetting[focus_second-1].MenuInfo.length() - 1);
            }
        }
        else if(ch == _KEY_CLR)
        {
            m_pNewSetting[focus_second-1].MenuInfo.clear();
        }
        else
        {
            if(0 < (cha = m_pMenuShareInfo->KeyValueConversion(ch)))
            {
                printf("----------input :%c\n",cha);
                m_pNewSetting[focus_second-1].MenuInfo.append(cha);
            }
        }
        menuStr = m_pNewSetting[focus_second-1].MenuInfo;
    }
}

void CMenuNet::RestartNetwork(void)
{
    m_pMenuShareInfo->m_ShareNetInfo.RemoteServer = m_pNewSetting[0].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.InternalServer = m_pNewSetting[1].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.Mac = m_pNewSetting[2].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.Ip = m_pNewSetting[3].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.Mask =  m_pNewSetting[4].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.Gateway = m_pNewSetting[5].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.Net_Mode = m_pNewSetting[6].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.SSID = m_pNewSetting[7].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.PassWord = m_pNewSetting[8].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.Encryption = m_pNewSetting[9].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.Dhcp = m_pNewSetting[10].MenuInfo;
    m_pMenuShareInfo->m_ShareNetInfo.Dns = m_pNewSetting[11].MenuInfo;

    m_pMenuShareInfo->StartNetwork();
}
void CMenuNet::updataStatusbar()
{
    switch(focus_second)
    {
        case 1:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","外服务器地址");
                break;
        case 2:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","内服务器地址");
                break;
        case 3:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","电子秤MAC地址,格式如 10:5A:04:AE:6E:98");
                break;
        case 4:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "电子秤IP电子,格式如 192.168.1.123");
                break;
        case 5:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "电子秤网络掩码,格式如 255.255.255.0");
                break;
        case 6:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "电子秤网关,格式如 192.168.1.1");
                break;
        case 7:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "0 有线网络, 1 无线网络");
                break;
        case 8:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "无线WIFI名称");
                break;
        case 9:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "无线WIFI密码");
                break;
        case 10:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "网络加密方式");
                break;
        case 11:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "0 不使用DHCP, 1 使用DHCP");
                break;
        case 12:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "DNS域名,格式如 8.8.8.8");
                break;
    }
}
