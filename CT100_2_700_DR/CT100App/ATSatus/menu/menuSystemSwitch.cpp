#include "menuSystemSwitch.h"

CMenuSystemSwitch::CMenuSystemSwitch(CShareInfo *m_pInfo)
{
    m_pMenuShareInfo = m_pInfo;
}
void CMenuSystemSwitch::init(void)
{
    QSqlQuery query;
    int dbSize;
    QString sqlStr = QString("select * from table_scalesetting where upper='%1';").arg(MENUSYSTEMSWITCH_UPPER);
    if(1 == (dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sqlStr,&query)))
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
    menuStr = m_pNewSetting[0].MenuInfo.left(1);
}
void CMenuSystemSwitch::Deal(void)
{
    init();
    printf("--------------------------------------------------------------CMenuSystemSwitch!\n");
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
                    printf("--------------------------------------------------------------CMenuSystemSwitch return mainMenu\n");
                    ReturnMainMenu();
                    return;
                 case _KEY_ENTER://确定,保存修改,这里写数据库
                    UpdataBar = true;
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
            if(0 == UpdataBar)
            {
                updataStatusbar();
            }
                updateMenuShow(m_pMenuShareInfo);
                m_bUpdate = false;
                UpdataBar = false;
        }
    }
}
int CMenuSystemSwitch::Sure(void)
{
    bool ok;
    switch(focus_second)
    {
        case 1:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.LoginMode = menuStr.toInt(&ok,10);
            break;
        case 2:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.PrintPaper = menuStr.toInt(&ok,10);
            break;
        case 3:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.CashSwitch = menuStr.toInt(&ok,10);
            break;
        case 4:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.CameraSwitch = menuStr.toInt(&ok,10);
            break;
        case 5:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.RfidSwitch = menuStr.toInt(&ok,10);
            break;
        case 6:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.BluetoothSwitch = menuStr.toInt(&ok,10);
            break;
        case 7:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.ZigbeeSwitch = menuStr.toInt(&ok,10);
            break;
        case 8:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.PosSwitch = menuStr.toInt(&ok,10);
            break;
        case 9:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.ScannerSwitch = menuStr.toInt(&ok,10);
            break;
        case 10:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.ManageSwitch = menuStr.toInt(&ok,10);
            break;
        case 11:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.WebSwitch = menuStr.toInt(&ok,10);
            break;
        case 12:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.AlipaySwitch = menuStr.toInt(&ok,10);
            break;
        case 13:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.WechatSwitch = menuStr.toInt(&ok,10);
            break;
        case 14:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.OtherSwitch = menuStr.toInt(&ok,10);
            break;
        case 15:
            m_pMenuShareInfo->m_ShareSysSwitchInfo.DebugSwitch = menuStr.toInt(&ok,10);
            break;
    }

    QString sqlStr = QString("update table_scalesetting  set value='%1' where id=%2;").arg(m_pNewSetting[0].MenuInfo).arg(m_pNewSetting[0].id);
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
}

void CMenuSystemSwitch::ReturnMainMenu()
{
    menuStr.clear();
    sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
    delete [] m_pNewSetting;
    m_pNewSetting = NULL;
}

void CMenuSystemSwitch::dec()
{
    if(focus_second>1)
    {
        focus_second--;
    }
    else
    {
        focus_second = focus_second+MENUSYSTEMSWITCH_SIZE-1;
    }
    printf("dec focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[0].MenuInfo.left(focus_second).right(1);
}

void CMenuSystemSwitch::add()
{
    if(focus_second<MENUSYSTEMSWITCH_SIZE)
    {
        focus_second++;
    }
    else{
        focus_second = focus_second-MENUSYSTEMSWITCH_SIZE+1;
    }
    printf("add focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[0].MenuInfo.left(focus_second).right(1);
}

void CMenuSystemSwitch::ProcessEditStr(unsigned char ch)
{
    char cha;
    if ((ch >= _KEY_NUM0) && (ch <= _KEY_NUM9)
        || (ch == _KEY_CLR))
    {
        if(ch == _KEY_CLR)
        {
            m_pNewSetting[0].MenuInfo.replace (focus_second-1,1,'0');
        }
        else
        {
            if(0 < (cha = m_pMenuShareInfo->KeyValueConversion(ch)))
            {
                printf("----------input :%c\n",cha);

                m_pNewSetting[0].MenuInfo.replace (focus_second-1,1,cha);
            }
        }
        menuStr = m_pNewSetting[0].MenuInfo.left(focus_second).right(1);
    }
}
void CMenuSystemSwitch::updataStatusbar()
{
    switch(focus_second)
    {
        case 1:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","0无登录界面,1帐号登录,2,刷卡登录,3帐号刷卡登录");
                break;
        case 2:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","0无小票,1默认1张小票,2两张小票");
                break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 14:
        case 15:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","(0 关闭)/(1 开启)该功能模块");
                break;
        case 12:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "0关闭,1链接支付宝,2生成展示码,3链接第三方");
                break;
        case 13:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "0关闭,1链接微信,2生成展示码,3链接第三方");
                break;
    }
}
