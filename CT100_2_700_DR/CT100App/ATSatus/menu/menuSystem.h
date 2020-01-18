#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H
#include "menu.h"
class CMenuSystem: public CMenu
{
public:
    CMenuSystem(CShareInfo *m_pInfo);
    ~CMenuSystem(void);
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

#endif // MENUSYSTEM_H
