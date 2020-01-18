#ifndef MENUSYSTEMSWITCH_H
#define MENUSYSTEMSWITCH_H
#include "menu.h"
class CMenuSystemSwitch:public CMenu
{
public:
    CMenuSystemSwitch(CShareInfo *m_pInfo);
    ~CMenuSystemSwitch(void);
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

#endif // MENUSYSTEMSWITCH_H
