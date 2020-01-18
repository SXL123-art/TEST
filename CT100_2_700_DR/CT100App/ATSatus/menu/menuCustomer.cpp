#include "menuCustomer.h"

CMenuCustomer::CMenuCustomer(CShareInfo *m_pInfo)
{
    m_pMenuShareInfo = m_pInfo;
}
void CMenuCustomer::init(void)
{
    QSqlQuery query;
    int dbSize;
    QString sqlStr = QString("select * from table_usersetting where upper='%1';").arg(MWNUCUSTOMER_UPPER);
    if(MENUCUSTOMER_SIZE == (dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(sqlStr,&query)))
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

void CMenuCustomer::Deal(void)
{
    init();
    printf("--------------------------------------------------------------CMenuCustomer!\n");
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
                    printf("--------------------------------------------------------------CMenuCustomer return mainMenu\n");
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
int CMenuCustomer::Sure(void)
{
    switch(focus_second)
    {
        case 1:
            m_pMenuShareInfo->m_ShareCustomerInfo.AlipayId = menuStr;
            break;
        case 2:
            m_pMenuShareInfo->m_ShareCustomerInfo.AlipayNo = menuStr;
            break;
        case 3:
            m_pMenuShareInfo->m_ShareCustomerInfo.WechatId = menuStr;
            break;
        case 4:
            m_pMenuShareInfo->m_ShareCustomerInfo.WechatNo = menuStr;
            break;
        case 5:
            m_pMenuShareInfo->m_ShareCustomerInfo.WechatKey = menuStr;
            break;
        case 6:
            m_pMenuShareInfo->m_ShareCustomerInfo.OtherID = menuStr;
            break;
        case 7:
            m_pMenuShareInfo->m_ShareCustomerInfo.OtherNo = menuStr;
            break;
        case 8:
            m_pMenuShareInfo->m_ShareCustomerInfo.BluetoothName = menuStr;
            break;
        case 9:
            m_pMenuShareInfo->m_ShareCustomerInfo.BluetoothPsd = menuStr;
            break;
        case 10:
            m_pMenuShareInfo->m_ShareCustomerInfo.ZigbeeNetID = menuStr;
            break;
        case 11:
            m_pMenuShareInfo->m_ShareCustomerInfo.ZigbeeChannel = menuStr;
            break;
    }

    QString sqlStr = QString("update table_usersetting  set value='%1' where id=%2;").arg(m_pNewSetting[focus_second-1].MenuInfo).arg(m_pNewSetting[focus_second-1].id);
    qDebug()<<sqlStr;
    if(ATdb_Excute_SQL_Statements(sqlStr)<=0)
    {
        qDebug() <<"update db fail-----------" << menuStr;
        SetStatusBar(m_pMenuShareInfo,0,menuStr);
    }
    else{
        SetStatusBar(m_pMenuShareInfo,1,menuStr);
    }
}

void CMenuCustomer::ReturnMainMenu()
{
    menuStr.clear();
    sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
    delete [] m_pNewSetting;
    m_pNewSetting = NULL;
}

void CMenuCustomer::dec()
{
    if(focus_second>1)
    {
        focus_second--;
    }
    else
    {
        focus_second = focus_second+MENUCUSTOMER_SIZE-1;
    }
    printf("dec focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}

void CMenuCustomer::add()
{
    if(focus_second<MENUCUSTOMER_SIZE)
    {
        focus_second++;
    }
    else{
        focus_second = focus_second-MENUCUSTOMER_SIZE+1;
    }
    printf("add focus_second--------------------%d\n",focus_second);
    menuStr = m_pNewSetting[focus_second-1].MenuInfo;
}
void CMenuCustomer::ProcessEditStr(unsigned char ch)
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

