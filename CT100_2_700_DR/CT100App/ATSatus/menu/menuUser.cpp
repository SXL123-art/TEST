#include "menuUser.h"
#
CMenuUser::CMenuUser(CShareInfo *m_pInfo)
{
    m_pMenuShareInfo = m_pInfo;
    focus_second = 1;
}

void CMenuUser::init(void)
{
    QSqlQuery query;
    int dbSize;
    QString sqlStr = QString("select * from table_usersetting where upper='%1';").arg(MENUUSER_UPPER);
    if(MENUUSER_SIZE == (dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sqlStr,&query)))
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
}

void CMenuUser::Deal(void)
{
    init();
    printf("--------------------------------------------------------------CMenuUser!\n");
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
                    printf("--------------------------------------------------------------CMenuUser return mainMenu\n");
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
int CMenuUser::Sure(void)
{
    if(false == m_pMenuShareInfo->m_nMenuFlag && (focus_second == 1 || focus_second == 5 || focus_second == 6))
    {
        return 0;
    }
    switch(focus_second)
    {
        case 1:
            m_pMenuShareInfo->m_ShareDeviceInfo.DeviceId = menuStr;
            break;
        case 2:
            m_pMenuShareInfo->m_ShareDeviceInfo.DeviceType = menuStr;
            break;
        case 3:
            m_pMenuShareInfo->m_ShareDeviceInfo.BoothId = menuStr;
            break;
        case 4:
            m_pMenuShareInfo->m_ShareDeviceInfo.BoothName = menuStr;
            break;
        case 5:
            m_pMenuShareInfo->m_ShareDeviceInfo.BoothCode = menuStr;
            break;
        case 6:
            m_pMenuShareInfo->m_ShareDeviceInfo.ScaleNum = menuStr;
            break;
        case 7:
            m_pMenuShareInfo->m_ShareDeviceInfo.MarketCode = menuStr;
            break;
        case 8:
            m_pMenuShareInfo->m_ShareDeviceInfo.MarketName = menuStr;
            break;
        case 9:
            m_pMenuShareInfo->m_ShareDeviceInfo.HeadTail1 = menuStr;
            break;
        case 10:
            m_pMenuShareInfo->m_ShareDeviceInfo.HeadTail2 = menuStr;
            break;
        case 11:
            m_pMenuShareInfo->m_ShareDeviceInfo.HeadTail3 = menuStr;
            break;
        case 12:
            m_pMenuShareInfo->m_ShareDeviceInfo.HeadTail4 = menuStr;
            break;
        case 13:
            m_pMenuShareInfo->m_ShareDeviceInfo.HeadTail5 = menuStr;
            break;
        case 14:
            m_pMenuShareInfo->m_ShareDeviceInfo.HeadTail6 = menuStr;
            break;
    }

    QString sqlStr = QString("update table_usersetting  set value='%1' where id=%2;").arg(m_pNewSetting[focus_second-1].MenuInfo).arg(m_pNewSetting[focus_second-1].id);
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

void CMenuUser::ReturnMainMenu()
{
    menuStr.clear();
    sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
    delete [] m_pNewSetting;
    m_pNewSetting = NULL;
}

void CMenuUser::dec()
{
    if(focus_second>1)
    {
        focus_second--;
    }
    else
    {
        focus_second = focus_second+MENUUSER_SIZE-1;
    }
    printf("dec focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}

void CMenuUser::add()
{
    if(focus_second<MENUUSER_SIZE)
    {
        focus_second++;
    }
    else{
        focus_second = focus_second-MENUUSER_SIZE+1;
    }
    printf("add focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}
void CMenuUser::ProcessEditStr(unsigned char ch)
{
    if(false == m_pMenuShareInfo->m_nMenuFlag && (focus_second == 1 || focus_second == 5 || focus_second == 6))
    {
        return ;
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

