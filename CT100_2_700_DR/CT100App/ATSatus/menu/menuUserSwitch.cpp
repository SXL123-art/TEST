#include "menuUserSwitch.h"

static bool flag = false;

CMenuUserSwitch::CMenuUserSwitch(CShareInfo *m_pInfo)
{
    m_pMenuShareInfo = m_pInfo;
}

void CMenuUserSwitch::init(void)
{
    QSqlQuery query;
    int dbSize;
    QString sqlStr = QString("select * from table_scalesetting where upper='%1';").arg(MENUUSERSWITCH_UPPER);
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
void CMenuUserSwitch::Deal(void)
{
    init();
    printf("--------------------------------------------------------------enter CMenuUserSwitch!\n");
    unsigned char ch;
    bool UpdataBar = false;
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
                    printf("--------------------------------------------------------------CMenuUserSwitch return mainMenu\n");
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
            if(false == UpdataBar)
            {
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
            }
            updateMenuShow(m_pMenuShareInfo);
            m_bUpdate = false;
            UpdataBar = false;
        }
    }
}
int CMenuUserSwitch::Sure(void)
{
    bool ok;
    switch(focus_second)
    {
        case 1:
            m_pMenuShareInfo->m_ShareUserSwitchInfo.TotalPriceSwitch = menuStr.toInt(&ok,10);
            break;
        case 2:
            m_pMenuShareInfo->m_ShareUserSwitchInfo.SavePluSwitch = menuStr.toInt(&ok,10);
            break;
        case 3:
            m_pMenuShareInfo->m_ShareUserSwitchInfo.SavePriceSwitch = menuStr.toInt(&ok,10);
            break;
        case 4:
            m_pMenuShareInfo->m_ShareUserSwitchInfo.RecordSwitch = menuStr.toInt(&ok,10);
            break;
        case 5:
            m_pMenuShareInfo->m_ShareUserSwitchInfo.PrintSwitch = menuStr.toInt(&ok,10);
            break;
        case 6:
            m_pMenuShareInfo->m_ShareUserSwitchInfo.fThresholdVale = ((menuStr.left(1).toInt(&ok,10))*10 + (menuStr.right(1).toInt(&ok,10)))/100.00;
            printf("m_pMenuShareInfo->m_ShareUserSwitchInfo.fThresholdVale:%f\n",m_pMenuShareInfo->m_ShareUserSwitchInfo.fThresholdVale);
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

void CMenuUserSwitch::ReturnMainMenu()
{
    flag = false;
    menuStr.clear();
    sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
    delete [] m_pNewSetting;
    m_pNewSetting = NULL;
}

void CMenuUserSwitch::dec()
{
    flag = false;
    if(focus_second>1)
    {
        focus_second--;
    }
    else
    {
        focus_second = focus_second+MENUUSERSWITCH_SIZE-1;
    }
    printf("dec focus_second--------------------%d\n",focus_second);
    if(6 == focus_second)
    {
        menuStr = m_pNewSetting[0].MenuInfo.right(2);
    }
    else{
        menuStr = m_pNewSetting[0].MenuInfo.left(focus_second).right(1);
    }
}

void CMenuUserSwitch::add()
{
    flag = false;
    if(focus_second<MENUUSERSWITCH_SIZE)
    {
        focus_second++;
    }
    else{
        focus_second = focus_second-MENUUSERSWITCH_SIZE+1;
    }
    printf("add focus_second--------------------%d\n",focus_second);
    if(6 == focus_second)
    {
        menuStr = m_pNewSetting[0].MenuInfo.right(2);
    }
    else{
        menuStr = m_pNewSetting[0].MenuInfo.left(focus_second).right(1);
    }
}

void CMenuUserSwitch::ProcessEditStr(unsigned char ch)
{
    char cha;
    if ((ch >= _KEY_NUM0) && (ch <= _KEY_NUM9)
        || (ch == _KEY_CLR))
    {
        if(ch == _KEY_CLR)
        {
            flag = false;
            if(6 == focus_second)
                m_pNewSetting[0].MenuInfo.replace (focus_second-1,2,"00");
            else
                m_pNewSetting[0].MenuInfo.replace (focus_second-1,1,'0');
        }
        else
        {
            if(0 < (cha = m_pMenuShareInfo->KeyValueConversion(ch)))
            {
                printf("----------input :%c\n",cha);
                if(6 == focus_second)
                {
                    if(flag == false)
                    {
                        m_pNewSetting[0].MenuInfo.replace (focus_second-1,1,cha);
                        flag = true;
                    }
                    else
                    {
                        m_pNewSetting[0].MenuInfo.replace (focus_second,1,cha);
                        flag = false;
                    }
                }
                else{
                    m_pNewSetting[0].MenuInfo.replace (focus_second-1,1,cha);
                }
            }
        }
        if(6 == focus_second)
        {
            menuStr = m_pNewSetting[0].MenuInfo.right(2);
        }
        else{
            menuStr = m_pNewSetting[0].MenuInfo.left(focus_second).right(1);
        }
    }
}
