#include "menu.h"

CMenu::CMenu()
{

}
CMenu::~CMenu()
{

}
void  CMenu::updateMenuShow(CShareInfo *m_pShareInfo)
{
    if(m_pShareInfo->shareInfoMenuUi.focus_child)
    {
        int len = menuStr.length();
        if(len >= 18)
            sprintf(m_pShareInfo->shareInfoMenuUi.menuData ,"%s", menuStr.right(18).toStdString().c_str());
        else
            sprintf(m_pShareInfo->shareInfoMenuUi.menuData ,"%s", menuStr.toStdString().c_str());
        m_pShareInfo->shareInfoMenuUi.focus_second = focus_second;
    }
    m_pShareInfo->UpdateGui();
}

void  CMenu::SetStatusBar(CShareInfo *m_pShareInfo,int type,QString str)
{
    if(1 == type)
    {
        sprintf(m_pShareInfo->shareInfoMenuUi.Statusbar ,"执行保存成功:%s", str.toStdString().c_str());
    }
    else{
        sprintf(m_pShareInfo->shareInfoMenuUi.Statusbar ,"执行保存失败:%s", str.toStdString().c_str());
    }
}
