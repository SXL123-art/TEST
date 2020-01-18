#ifndef MENUCUSTOMER_H
#define MENUCUSTOMER_H
#include "menu.h"

class CMenuCustomer:public CMenu
{
public:
    CMenuCustomer(CShareInfo *m_pInfo);
    ~CMenuCustomer(void);
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

#endif // MENUCUSTOMER_H
