#ifndef MENUUSERSWITCH_H
#define MENUUSERSWITCH_H
#include "menu.h"
class CMenuUserSwitch:public CMenu
{
public:
    CMenuUserSwitch(CShareInfo *m_pInfo);
    ~CMenuUserSwitch(void);
    void init(void);
     void Deal(void);
     int Sure(void);
     void ReturnMainMenu();
      void updataStatusbar();
      void dec();
      void add();
      void ProcessEditStr(unsigned char ch);

private:
     CShareInfo*         m_pMenuShareInfo;
     m_MenuInfo * 	m_pNewSetting;
     bool m_bUpdate;
};

#endif // MENUUSERSWITCH_H
