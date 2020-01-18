#include "login.h"


CLogin::CLogin(CShareInfo *pInfo)
{
    m_pLoginShareInfo = pInfo;
    m_bUpdate = false;
    sprintf(m_pLoginShareInfo->shareInfoLoginUi.hdversion,"%s","28_V3.2");
    sprintf(m_pLoginShareInfo->shareInfoLoginUi.swversion,"%s","CT100_2_V1.0");
}

CLogin::~CLogin(void)
{

}
int CLogin::LoginInit(void)
{
    m_pLoginShareInfo->m_enQtUiType = enQt_LoginQT;
    memset(m_pLoginShareInfo->shareInfoLoginUi.user, 0, LEN32);
    memset(m_pLoginShareInfo->shareInfoLoginUi.psw, 0, LEN32);
    m_pLoginShareInfo->UpdateGui();
    m_inputStep = false;
    m_stInputUser.clear();
    m_strPasswd.clear();
}

enQtShowType CLogin::LoginProc()
{
    printf("-------------------------------------------CLogin\n");
    m_pLoginShareInfo->m_enQtUiType = enQt_LoginQT;

    LoginInit();

    while(1)
    {
        usleep(50*1000);
        if(m_pLoginShareInfo->m_enQtUiType == enQt_MsgQT)
        {
            continue;
        }
        switch (LoginDeal())
        {
        case enQt_LoginQT:
                // 退回登陆界
                break ;

        case enQt_SaleQT:
                //
                return enQt_SaleQT;

        case enQt_MenuQT:
                // 配置界面
                return enQt_MenuQT;
        case enQt_CalQt:
                // 标定界面
                return enQt_CalQt;
        }
        UpdateLoginShow();
    }
}

enQtShowType CLogin::LoginDeal()
{
    int nRet = 0;
    //验证帐号和密码登录
    if(m_pLoginShareInfo->m_ShareSysSwitchInfo.LoginMode == 1)
    {
        nRet = UserPswLogin();
        if(10 == nRet)
        {
            return enQt_MenuQT;
        }
        else if(8 == nRet)
        {
            return enQt_CalQt;
        }
        else if(nRet>0)
        {
            return enQt_SaleQT;
        }
    }
    else if(m_pLoginShareInfo->m_ShareSysSwitchInfo.LoginMode == 2)
    {
        //刷卡登录
        if(LoginCardDeal()>0)
        {
            return enQt_SaleQT;
        }
    }
    else if(m_pLoginShareInfo->m_ShareSysSwitchInfo.LoginMode == 3)
    {
        //输入密码账号和刷卡
        nRet = UserPswLogin();
        if(10 == nRet)
        {
            return enQt_MenuQT;
        }
        else if(8 == nRet)
        {
            return enQt_CalQt;
        }
        else if(nRet>0)
        {
            return enQt_SaleQT;
        }

        if(LoginCardDeal() > 0)
        {
            printf("LoginInputDeal or LoginCardDeal:-----------------------\n");
            return enQt_SaleQT;
        }
    }
    else
    {
        return enQt_SaleQT;
    }
    return enQt_LoginQT;
}
int CLogin::UserPswLogin(void)
{
    unsigned char ch = 0;
    usleep(100*1000);
    if (SALE_KEY_VALID == ATKey_getKey(&ch))
    {
       m_bUpdate = true;
       EditString(ch);
       switch(ch)
       {
           case _KEY_MENU:
               if(showInputMsg(enInputPasswd,"请输入菜单密码")>0)
               {
                   return 10;
               }
               break;
          case _KEY_ENTER:
               if (CheckLoginUserAndPasswd() == 1)
               {
                   return 1;
               }
               break;
           case _KEY_CANCEL:
                m_inputStep = false;
                m_stInputUser.clear();
                m_strPasswd.clear();
                //sprintf(m_pLoginShareInfo->shareInfoLoginUi.user, "%s", "|");
           case _KEY_ZERO:
           case _KEY_TARE:
               // 者去皮
               m_pLoginShareInfo->ZeroTare(ch);
               break;
       }
    }
    return 0;
}
int CLogin::CheckLoginUserAndPasswd(void)
{
    //用户名输入
    if(m_inputStep == false)
    {
        if(m_stInputUser.length()<=0)
        {
            sprintf(m_pLoginShareInfo->shareInfoLoginUi.message,"%s","用户名输入不能为空");
            return 0;
        }
        QString Qstr;
        ATdb_Select_ReturnQString_By_SQL_Statements(QString("select name from table_sellersinfo where name='%1';").arg(m_stInputUser),&Qstr);
        if(0 == QString::compare(m_stInputUser,"cas") || Qstr.length()>0)
        {
            m_inputStep = true;
            //sprintf(m_pLoginShareInfo->shareInfoLoginUi.psw, "%s", "|");
        }
        else
        {
            sprintf(m_pLoginShareInfo->shareInfoLoginUi.message,"%s","输入错误,无该用户名");
            m_stInputUser.clear();
        }
    }
    else
    {
        if(m_strPasswd.length()<=0)
        {
            sprintf(m_pLoginShareInfo->shareInfoLoginUi.message,"%s","登录密码输入不能为空");
            return 0;
        }
        if(0 == QString::compare(m_stInputUser,"cas") && 0 == QString::compare(m_strPasswd,"123456"))
        {
            return 1;
        }
        QString Qstr;
        ATdb_Select_ReturnQString_By_SQL_Statements(QString("select pass from table_sellersinfo where name='%1';").arg(m_stInputUser),&Qstr);
        if(m_stInputUser == Qstr)
        {
            m_inputStep = false;
            return 1;
        }
        else
        {
            sprintf(m_pLoginShareInfo->shareInfoLoginUi.message,"%s","用户名与密码不匹配,请重新输入密码");
            m_strPasswd.clear();
        }
    }
}
int CLogin::LoginCardDeal(void)
{
   return m_pLoginShareInfo->ReadCPUCardInfo();
}
int CLogin::EditString(unsigned char ch)
{
    char cha;
    if ((ch>=_KEY_PLU1) && (ch<=_KEY_PLU60)
        ||((ch>=_KEY_NUM0) && (ch<=_KEY_NUM9)
        || (ch==_KEY_DOT)
        || (ch == _KEY_BACK)
        || (ch == _KEY_CLR)
        || (ch == _KEY_SHIFT)))
    {
        if(ch == _KEY_CLR)
        {
            if(false == m_inputStep)
            {
                m_stInputUser.clear();
            }
            else
            {
                m_strPasswd.clear();
            }
        }
        else if(ch == _KEY_BACK)
        {
            if(false == m_inputStep)
            {
                if(m_stInputUser.length()>0)
                {
                    m_stInputUser = m_stInputUser.left(m_stInputUser.length() - 1);
                }
            }
            else
            {
                if(m_strPasswd.length()>0)
                {
                    m_strPasswd = m_strPasswd.left(m_strPasswd.length() - 1);
                }
            }
        }
        else
        {
            if(0 < (cha = m_pLoginShareInfo->KeyValueConversion(ch)))
            {
                printf("----------input :%c\n",cha);
                if(false == m_inputStep)
                {
                    m_stInputUser.append(cha);
                }
                else
                {
                    m_strPasswd.append(cha);
                }
            }
        }
    }
}

int CLogin::UpdateLoginShow(void)
{
    if (m_bUpdate)
    {
        m_bUpdate = false;
        m_pLoginShareInfo->shareInfoLoginUi.select = m_inputStep;
        sprintf(m_pLoginShareInfo->shareInfoLoginUi.user,"%s",m_stInputUser.toStdString().c_str());
        sprintf(m_pLoginShareInfo->shareInfoLoginUi.psw,"%s",m_strPasswd.toStdString().c_str());
        m_pLoginShareInfo->UpdateGui();
        sprintf(m_pLoginShareInfo->shareInfoLoginUi.message,"%s","");
    }
    else
    {
        // 重量有变化
        if (m_pLoginShareInfo->RefreshWeight() > 0)
        {
            sprintf(m_pLoginShareInfo->shareInfoLoginUi.weight, "%s", m_pLoginShareInfo->shareInfoSaleUi.weight);
            sprintf(m_pLoginShareInfo->shareInfoLoginUi.tare, "%s", m_pLoginShareInfo->shareInfoSaleUi.tare);

            m_pLoginShareInfo->shareInfoLoginUi.stable = m_pLoginShareInfo->shareInfoSaleUi.stable;
            m_pLoginShareInfo->shareInfoLoginUi.net = m_pLoginShareInfo->shareInfoSaleUi.net;
            m_pLoginShareInfo->shareInfoLoginUi.zero = m_pLoginShareInfo->shareInfoSaleUi.zero;
            m_pLoginShareInfo->UpdateGui();
        }
    }
}
int CLogin::showInputMsg(enInputType InputType, char *pInputMSG)
{
    int nRet = showInputMsgProc(m_pLoginShareInfo,InputType,pInputMSG);
    sprintf(m_pLoginShareInfo->m_stInputInfo.input,"%s","");
    printf("-------------------------------------------CLogin\n");
    m_pLoginShareInfo->m_enQtUiType = enQt_LoginQT;
    if(10 == nRet)
        return 10;
    m_pLoginShareInfo->m_enQtUiType = enQt_LoginQT;
    m_pLoginShareInfo->UpdateGui();
    return nRet;
}
