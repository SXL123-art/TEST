#ifndef MENU_H
#define MENU_H
#include "../../unit/shareinfo.h"
#include "../../public/typeMenu.h"
#include "../msgdialog/msgdialog.h"

class CMenu
{
public:
    CMenu();
    ~CMenu(void);

    virtual void Deal(void){};
    void  updateMenuShow(CShareInfo *m_pShareInfo);
    void  SetStatusBar(CShareInfo *m_pShareInfo,int type,QString str);

    QString    menuStr;
    unsigned char   focus_second;
};

#endif // MENU_H
