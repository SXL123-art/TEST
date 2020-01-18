#include "mainmenu.h"

CMainMenu::CMainMenu(CShareInfo *pInfo)
{
    m_pMenuShareInfo = pInfo;
    printf("-------------------------------------------init CMainMenu\n");
    m_pMenuNet = new CMenuNet(m_pMenuShareInfo);
    m_pMenuUser = new CMenuUser(m_pMenuShareInfo);
    m_pMenuCustomer = new CMenuCustomer(m_pMenuShareInfo);
    m_pMenuSystem = new CMenuSystem(m_pMenuShareInfo);
    m_pMenuDataManagement = new CMenuDataManagement(m_pMenuShareInfo);
    m_pMenuUserSwitch = new CMenuUserSwitch(m_pMenuShareInfo);
    m_pMenuSystemSwitch = new CMenuSystemSwitch(m_pMenuShareInfo);

    focus_parent = 1;
    m_pMenuShareInfo->shareInfoMenuUi.focus_parent = focus_parent;
    m_pMenuShareInfo->shareInfoMenuUi.focus_second = 1;
    m_bUpdate = false;
}
CMainMenu::~CMainMenu(void)
{

}

enQtShowType CMainMenu::MainMenuProc()
{
    printf("--------------------------------------------------------------CMainMenu\n");
    m_pMenuShareInfo->shareInfoMenuUi.focus_child = false;
    focus_parent = 1;
    m_pMenuShareInfo->shareInfoMenuUi.focus_parent = 1;
    m_pMenuShareInfo->m_enQtUiType = enQt_MenuQT;
    m_pMenuShareInfo->UpdateGui();
    while(true)
    {
        usleep(50*1000);

        if(enQt_MenuQT == MainMenuDeal())
        {
                continue;
        }
        else
        {
            return m_pMenuShareInfo->enLastShowType;
        }
    }
}

enQtShowType CMainMenu::MainMenuDeal()
{
    unsigned char ch = 0;
    if (SALE_KEY_VALID == ATKey_getKey(&ch))
    {
        m_bUpdate = true;
        switch(ch)
        {
            case _KEY_MENU:
                return m_pMenuShareInfo->enLastShowType;
            case _KEY_UP:
                dec();    
                break;
            case _KEY_DOWN:
                add();
                break;
            case _KEY_ENTER://确定
                ModifyChildValue();
                ChildProc();         
                break;
            case _KEY_NUM1:
            case _KEY_NUM2:
            case _KEY_NUM3:
            case _KEY_NUM4:
            case _KEY_NUM5:
            case _KEY_NUM6:
            case _KEY_NUM7:
                KeyModify(ch);
            break;
        } 
    }
    if(m_bUpdate)
    {
        m_bUpdate = false;
        m_pMenuShareInfo->UpdateGui();
    }
    return enQt_MenuQT;
}

void CMainMenu::ChildProc()
{
    printf("--------------------------------------------------------------CMenuMain ChildProc: %d!\n", focus_parent);

    CMenu* pMenu = NULL;

    switch(focus_parent)
    {
    case 1:
            pMenu = m_pMenuNet;
            break;
    case 2:
            pMenu = m_pMenuUser;
            break;
    case 3:
            pMenu = m_pMenuCustomer;
            break;
    case 4:
            pMenu = m_pMenuSystem;
            break;
    case 5:
            pMenu = m_pMenuDataManagement;
            break;
    case 6:
            pMenu = m_pMenuUserSwitch;
            break;
    case 7:
            pMenu = m_pMenuSystemSwitch;
            break;
    }
    pMenu->Deal();
    ModifyParentValue();
}
 void CMainMenu::add()
 {
     if(focus_parent<MAINMENU_SIZE)
     {
         focus_parent++;
     }
     else
     {
         focus_parent = focus_parent-MAINMENU_SIZE+1;
     }
     printf("add focus_parent--------------------%d\n",focus_parent);
     m_pMenuShareInfo->shareInfoMenuUi.focus_parent = focus_parent;
 }

 void CMainMenu::dec()
 {
     if(focus_parent>1)
     {
         focus_parent--;
     }
     else
     {
         focus_parent = focus_parent+MAINMENU_SIZE-1;
     }
     printf("dec focus_parent--------------------%d\n",focus_parent);
     m_pMenuShareInfo->shareInfoMenuUi.focus_parent = focus_parent;
 }
 void CMainMenu::KeyModify(unsigned char ch)
 {
     focus_parent = ch - _KEY_NUM0;
     //printf("KeyModify focus_parent--------------------%d\n",focus_parent);
     m_pMenuShareInfo->shareInfoMenuUi.focus_parent = focus_parent;
 }
 void CMainMenu::ModifyChildValue(void)
 {
     m_pMenuShareInfo->shareInfoMenuUi.focus_parent = focus_parent;
     m_pMenuShareInfo->shareInfoMenuUi.focus_second = 1;
     m_pMenuShareInfo->shareInfoMenuUi.focus_child = true;
 }
 void CMainMenu::ModifyParentValue(void)
 {
     m_pMenuShareInfo->shareInfoMenuUi.focus_parent = focus_parent;
     m_pMenuShareInfo->shareInfoMenuUi.focus_second = 1;
     m_pMenuShareInfo->shareInfoMenuUi.focus_child = false;
     sprintf(m_pMenuShareInfo->shareInfoMenuUi.Statusbar,"%s","");
 }
