#ifndef MENUUSER_H
#define MENUUSER_H
#include "menu.h"
class CMenuUser:public CMenu
{
public:
    CMenuUser(CShareInfo *m_pInfo);
    ~CMenuUser(void);
    void init(void);
     void Deal(void);
     int Sure(void);
     void ReturnMainMenu();
      void dec();
      void add();
      void ProcessEditStr(unsigned char ch);

private:
     CShareInfo*         m_pMenuShareInfo;
     m_MenuInfo * 	m_pNewSetting;
     bool m_bUpdate;
};

#endif // MENUUSER_H
