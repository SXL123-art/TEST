#ifndef LOGIN_H
#define LOGIN_H
#include "../../unit/shareinfo.h"
#include "../msgdialog/msgdialog.h"
#include "../baseclass.h"

//#define  PASSWORDLEN 32

class CLogin
{
public:
    CLogin(CShareInfo *pInfo);

    ~CLogin(void);

    enQtShowType LoginProc();
private:
    enQtShowType LoginDeal();
    int LoginInit(void);
    int UpdateLoginShow(void);
    int UserPswLogin(void);
    int CheckLoginUserAndPasswd(void);
    int LoginCardDeal(void);
    int EditString(unsigned char ch);

    int showInputMsg(enInputType InputType, char *pInputMSG);
    bool m_bUpdate;

public:
    CShareInfo *m_pLoginShareInfo;

    bool m_inputStep;

    QString	m_stInputUser;
    QString	m_strPasswd;
};

#endif // LOGIN_H
