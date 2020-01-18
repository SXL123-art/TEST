#include "menuDataManagement.h"

CMenuDataManagement::CMenuDataManagement(CShareInfo *m_pInfo)
{
    m_pMenuShareInfo = m_pInfo;
}
void CMenuDataManagement::init(void)
{
    focus_second = 1;
}

void CMenuDataManagement::Deal(void)
{
    init();
    printf("--------------------------------------------------------------CMenuDataManagement!\n");
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
            //解析按键值
            switch(ch)
            {
                case _KEY_MENU:	//返回上一级菜单
                    printf("--------------------------------------------------------------CMenuDataManagement return mainMenu\n");
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
            m_pMenuShareInfo->UpdateGui();
            m_bUpdate = false;
            UpdataBar = false;
        }
    }
}
int CMenuDataManagement::Sure(void)
{
    int nRet = 0;
    switch(focus_second)
    {
        case 1:	// 清除交易信息
          if(ATdb_Excute_SQL_Statements("delete from table_attach;")<0)
          {
              break;
          }
          if(ATdb_Excute_SQL_Statements("delete from table_business;")<0)
          {
              break;
          }
          nRet = 1;
          break;
        case 2: //清除商品信息
            if(ATdb_Excute_SQL_Statements("delete from table_product;")<0)
            {
                break;
            }
            if(ATdb_Excute_SQL_Statements("delete from table_convert;")<0)
            {
                break;
            }
            nRet = 1;
            break;
        case 3: //清除批次信息
            if(ATdb_Excute_SQL_Statements("delete from table_batch;")<0)
            {
                break;
            }
            nRet = 1;
            break;
        case 4: //清除快捷键
            if(ATdb_Excute_SQL_Statements("delete from table_convert;")<0)
            {
                break;
            }
            nRet = 1;
            break;
        case 5: //清除操作员信息
            if(ATdb_Excute_SQL_Statements("delete from table_sellersinfo;")<0)
            {
                break;
            }
            nRet = 1;
            break;
        case 6: //初始化商品信息
            nRet = m_pMenuShareInfo->PerformSqlFile("./plu_sql.txt");
            break;
        case 7: //初始化必要菜单参数
            nRet = m_pMenuShareInfo->PerformSqlFile("./setting_sql.txt");
            break;
        default:
                break;
    }
    if(nRet>0)
    {
        SetStatusBar(m_pMenuShareInfo,1,"");
    }
    else{
        SetStatusBar(m_pMenuShareInfo,0,"");
    }
}

void CMenuDataManagement::ReturnMainMenu()
{
    menuStr.clear();
    sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
}

void CMenuDataManagement::dec()
{
    if(focus_second>1)
    {
        focus_second--;
    }
    else
    {
        focus_second = focus_second+MENUDATA_SIZE-1;
    }
    printf("dec focus_second--------------------%d\n",focus_second);
    m_pMenuShareInfo->shareInfoMenuUi.focus_second = focus_second;
}

void CMenuDataManagement::add()
{
    if(focus_second<MENUDATA_SIZE)
    {
        focus_second++;
    }
    else{
        focus_second = focus_second-MENUDATA_SIZE+1;
    }
    printf("add focus_second--------------------%d\n",focus_second);
    m_pMenuShareInfo->shareInfoMenuUi.focus_second = focus_second;
}


