#include "menuSystem.h"

CMenuSystem::CMenuSystem(CShareInfo *m_pInfo)
{
    m_pMenuShareInfo = m_pInfo;
}

void CMenuSystem::init(void)
{
    QSqlQuery query;
    int dbSize;
    QString sqlStr = QString("select * from table_scalesetting where upper='%1';").arg(MENUSYSTEM_UPPER);
    if((MENUSYSTEM_SIZE-1) == (dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sqlStr,&query)))
    {
        m_pNewSetting = new m_MenuInfo[dbSize+1];
        bool ok;
        for(int i=1; i<dbSize+1;i++)
        {
            query.next();
            m_pNewSetting[i].id = query.record().value(TABLE_SCALESETTING_ID).toString().toInt(&ok,10);
            m_pNewSetting[i].MenuInfo = query.record().value(TABLE_SCALESETTING_VALUE).toString();
        }
    }
    focus_second = 1;
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}

void CMenuSystem::Deal()
{
    init();

    printf("--------------------------------------------------------------CMenuSystem\n");
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
                    printf("--------------------------------------------------------------CMenuSystem return mainMenu\n");
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
int CMenuSystem::Sure(void)
{
    bool ok;
    int len = menuStr.length();
    switch(focus_second)
    {
        case 1:
            //更新系统时间
            if(len == 12)
            {
                ATRtcWrite((char*)menuStr.toStdString().c_str());
                SetStatusBar(m_pMenuShareInfo,1,menuStr);
                break;
            }
            SetStatusBar(m_pMenuShareInfo,0,menuStr);
            break;
        case 2:
            m_pMenuShareInfo->m_ShareSystemInfo.ScreenTimeout = menuStr.toInt(&ok,10);
            break;
        case 3:
            //m_pMenuShareInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT = menuStr.toInt(&ok,10);
            break;
        case 4:
            m_pMenuShareInfo->m_ShareSystemInfo.Round = menuStr.toInt(&ok,10);
            break;
        case 5:
            m_pMenuShareInfo->m_ShareSystemInfo.BackLight = menuStr.toInt(&ok,10);
            break;
    }

    if(focus_second != 1)
    {
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
    }
}

void CMenuSystem::ReturnMainMenu()
{
    menuStr.clear();
    sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
    delete [] m_pNewSetting;
    m_pNewSetting = NULL;
}

void CMenuSystem::dec()
{
    if(focus_second>1)
    {
        focus_second--;
    }
    else
    {
        focus_second = focus_second+MENUSYSTEM_SIZE-1;
    }
    printf("dec focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}

void CMenuSystem::add()
{
    if(focus_second<MENUSYSTEM_SIZE)
    {
        focus_second++;
    }
    else{
        focus_second = focus_second-MENUSYSTEM_SIZE+1;
    }
    printf("add focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}

void CMenuSystem::ProcessEditStr(unsigned char ch)
{
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
void CMenuSystem::updataStatusbar()
{
    switch(focus_second)
    {
        case 1:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","设置时间,格式为:180321120930");
                break;
        case 2:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","超时锁屏时间,单位:秒");
                break;
        case 3:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","重量去皮小数点(取值2,3)(重启生效)");
                break;
        case 4:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "0不舍,1舍分,2舍角");
                break;
        case 5:
                sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s", "显示屏亮度等级(取值0-9)");
                break;
    }
}
