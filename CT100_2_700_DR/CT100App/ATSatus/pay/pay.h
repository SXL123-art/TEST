#ifndef PAY_H
#define PAY_H

#include "../../unit/shareinfo.h"
#include "../msgdialog/msgdialog.h"
#include "business.h"

class CPay
{
public:
    CPay(CShareInfo *pInfo);

    ~CPay(void);

    enQtShowType PayProc();
private:
    enQtShowType PayDeal();
    int UpdatePayShow(void);
    int CheckPayStatus(void);
    int showInputMsg(enInputType InputType, char *pInputMSG);
    int showMsg(enMsgType MsgType, char *pTitle,char *pMSG);
    int ClearPayInfo(void);
    void ChangePayMode(enPayMode mode);
    bool m_bUpdate;

public:
    CShareInfo *m_pPayShareInfo;

    CBusiness 	*m_pBusiness;
};

#endif // PAY_H
