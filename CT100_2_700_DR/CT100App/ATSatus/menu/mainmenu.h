#ifndef MAINMENU_H
#define MAINMENU_H
#include "../../unit/shareinfo.h"
#include "../../public/typeMenu.h"
#include "menu.h"

#include "menuNet.h"
#include "menuUser.h"
#include "menuCustomer.h"
#include "menuSystem.h"
#include "menuDataManagement.h"
#include "menuUserSwitch.h"
#include "menuSystemSwitch.h"


class CMainMenu
{
public:
    CMainMenu(CShareInfo *m_pInfo);
    ~CMainMenu(void);
    void init(void);

    enQtShowType MainMenuProc();

    enQtShowType MainMenuDeal();

private:

    void ChildProc();
    void add();
    void dec();
    void ModifyChildValue(void);
    void ModifyParentValue(void);
    void KeyModify(unsigned char ch);

    CMenuNet*  m_pMenuNet;
    CMenuUser*  m_pMenuUser;
    CMenuCustomer*  m_pMenuCustomer;
    CMenuSystem*   m_pMenuSystem;
    CMenuDataManagement* m_pMenuDataManagement;
    CMenuUserSwitch* m_pMenuUserSwitch;
    CMenuSystemSwitch* m_pMenuSystemSwitch;

    CShareInfo*         m_pMenuShareInfo;
    unsigned  char   focus_parent;
    bool   m_bUpdate ;
};

#endif // MAINMENU_H
