#ifndef MENUDATAMANAGEMENT_H
#define MENUDATAMANAGEMENT_H
#include "menu.h"
class CMenuDataManagement:public CMenu
{
public:
    CMenuDataManagement(CShareInfo *m_pInfo);
    ~CMenuDataManagement(void);
    void init(void);
     void Deal(void);
     int Sure(void);
     void ReturnMainMenu();
      void dec();
      void add();
private:
     bool m_bUpdate;
     CShareInfo*         m_pMenuShareInfo;
};

#endif // MENUDATAMANAGEMENT_H
