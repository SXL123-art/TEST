#ifndef MENUNET_H
#define MENUNET_H
#include "menu.h"



class CMenuNet:public CMenu
{
public:
    CMenuNet(CShareInfo *m_pInfo);
    ~CMenuNet(void);
    void init(void);
     void Deal(void);
     int Sure(void);
     void RestartNetwork(void);
    void ReturnMainMenu();
     void updataStatusbar();
     void dec();
     void add();
     void KeyModify(unsigned char ch);
     void ProcessEditStr(unsigned char ch);

private:
     CShareInfo*         m_pMenuShareInfo;
     m_MenuInfo * 	m_pNewSetting;
     bool m_bUpdate;
     bool		m_bSetSure;
};

#endif // MENUNET_H
