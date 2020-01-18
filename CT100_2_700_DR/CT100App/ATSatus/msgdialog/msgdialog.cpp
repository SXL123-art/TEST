#include "msgdialog.h"

#define PASSWORDLEN 64
char m_passwdCard[PASSWORDLEN] = {0};

int showMsgProc(CShareInfo *m_pShareInfo,enMsgType MsgType, char *pTitle,char *pMSG)
{
    m_pShareInfo->m_enQtUiType = enQt_MsgQT;
    m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_REQ;	//请求还是确认消息
    SetInputContent(m_pShareInfo,pTitle,pMSG);
    m_pShareInfo->UpdateGui();

    unsigned char ch;
    bool m_bUpdate = false;
    printf("-------------------------------------------showMsg\n");
    while(1)
    {
        usleep(100*1000);
        if (SALE_KEY_VALID == ATKey_getKey(&ch))
        {
           m_bUpdate = true;
           //根据按键值做出相应操作
            switch(ch)
            {
                case _KEY_CANCEL:
                    if(MsgType == enMsgPay)
                    {
                        m_pShareInfo->m_ShareSaleInfo.SaleStatus = enPayCancel;
                        break;
                    }
                    else if(MsgType == enMsgFx || MsgType == enMsgFn || MsgType == enMsgRfidTest)
                    {
                        m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_OK;
                        m_pShareInfo->UpdateGui();
                        return -1;
                    }
                    break;
                case _KEY_ENTER:
                    if(MsgType == enMsgPay)
                    {
                        if(m_pShareInfo->GetPayMode() != enCash)
                        {
                            m_pShareInfo->m_ShareSaleInfo.SaleStatus = enPayCancel;
                        }
                        m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_OK;
                        m_pShareInfo->UpdateGui();
                        return 11;
                    }
                    else if(MsgType == enMsgFx || MsgType == enMsgFn)
                    {
                        m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_OK;
                        m_pShareInfo->UpdateGui();
                        return 10;
                    }
                    break;
                /*case _KEY_PRINT:
                    if(MsgType == enMsgFx || MsgType == enMsgFn)
                    {
                        m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_OK;
                        m_pShareInfo->UpdateGui();
                        return 10;
                    }
                    break;*/
                case _KEY_CLR:
                    if(MsgType == enMsgFx || MsgType == enMsgFn)
                    {
                        m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_OK;
                         m_pShareInfo->UpdateGui();
                        return -10;
                    }
                    m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_OK;
                    m_pShareInfo->UpdateGui();
                    break;
                case _KEY_PAY_MODE:
                    if(MsgType == enMsgFx || MsgType == enMsgFn)
                    {
                        m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_OK;
                         m_pShareInfo->UpdateGui();
                        return 5;
                    }
                default:break;

            }
        }
        if(ProcessMessage(m_pShareInfo,MsgType)>0)
        {
            m_pShareInfo->m_stMessageInfo.cmd = MSG_MESSAGE_OK;
            m_pShareInfo->UpdateGui();
            return 2;
        }
    }
}

void SetInputContent(CShareInfo *m_pShareInfo,const char *pTitle,const char *pData)
{
    //转码
    if(m_pShareInfo->m_enQtUiType == enQt_MsgQT)
    {
        sprintf(m_pShareInfo->m_stMessageInfo.title,"%s",pTitle);
        sprintf(m_pShareInfo->m_stMessageInfo.message,"%s",pData);

    }
    else if(m_pShareInfo->m_enQtUiType == enQt_InputQT)
    {
        sprintf(m_pShareInfo->m_stInputInfo.message,"%s",pTitle);
    }
}

int ProcessMessage(CShareInfo *m_pShareInfo,enMsgType MsgType)
{
    //根据状态做出相应操作
    if(MsgType == enMsgRfidTest)
    {
        if(m_pShareInfo->Rfid_Find() >0)
            return 1;
    }
    if(MsgType == enMsgFn)
    {
        if(m_pShareInfo->GetSaleMode() == enPieces)
        {
            return 0;
        }
        if ((m_pShareInfo->m_adInfo.kg>=0 && m_pShareInfo->m_adInfo.kg<=10)
                || (m_pShareInfo->m_adInfo.kg < 0))
        {
            // 重量变化后
            printf("==================================================>Fx zero\n");
            return 1;
        }
    }
    else if(MsgType == enMsgPay)
    {
        if(enPayUnSupport == m_pShareInfo->m_ShareSaleInfo.SaleStatus)
        {
            // 支付方式不支持，2S后自动返回
            m_pShareInfo->m_ShareSaleInfo.SaleStatus = enPayCancel;
            usleep(2000*1000);

            return 1;
        }
        else if(enPaySuccess == m_pShareInfo->m_ShareSaleInfo.SaleStatus)
        {
            if (m_pShareInfo->m_enPayMode == enUnionPay)
            {
                    SetInputContent(m_pShareInfo,"银联卡支付", "银联卡支付成功...");
            }
            else if (m_pShareInfo->m_enPayMode == enAliPay)
            {
                    SetInputContent(m_pShareInfo, "支付宝支付","支付宝支付成功...");
            }
            else if (m_pShareInfo->m_enPayMode == enWeChatPay)
            {
                    SetInputContent(m_pShareInfo, "微信支付","微信支付成功...");
            }
            else if (m_pShareInfo->m_enPayMode == enVipPay)
            {
                    SetInputContent(m_pShareInfo, "会员卡支付","会员卡支付成功...");
            }
            else if (m_pShareInfo->m_enPayMode == enBankPay)
            {
                    SetInputContent(m_pShareInfo, "银行支付","银行支付成功...");
            }
            m_pShareInfo->UpdateGui();
            // 支付成功，2S后自动开始打印签购单
            usleep(2000*1000);
            return 1;
        }
        else if(enPayCancel == m_pShareInfo->m_ShareSaleInfo.SaleStatus)
        {
            if (m_pShareInfo->m_enPayMode == enUnionPay)
            {
                    SetInputContent(m_pShareInfo,"银联卡支付", "银联卡支付取消...");
            }
            else if (m_pShareInfo->m_enPayMode == enAliPay)
            {
                    SetInputContent(m_pShareInfo, "支付宝支付","支付宝支付取消...");
            }
            else if (m_pShareInfo->m_enPayMode == enWeChatPay)
            {
                    SetInputContent(m_pShareInfo, "微信支付","微信支付取消...");
            }
            else if (m_pShareInfo->m_enPayMode == enVipPay)
            {
                    SetInputContent(m_pShareInfo, "会员卡支付","会员卡支付取消...");
            }
            else if (m_pShareInfo->m_enPayMode == enBankPay)
            {
                    SetInputContent(m_pShareInfo, "扫码支付","扫码支付取消...");
            }
            m_pShareInfo->UpdateGui();
            // 支付成功，2S后自动开始打印签购单
            usleep(2000*1000);
            return 1;
        }
        else if(enPayFail == m_pShareInfo->m_ShareSaleInfo.SaleStatus)
        {
            if (m_pShareInfo->m_enPayMode == enUnionPay)
            {
                    SetInputContent(m_pShareInfo,"银联卡支付", "银联卡支付失败...");
            }
            else if (m_pShareInfo->m_enPayMode == enAliPay)
            {
                    SetInputContent(m_pShareInfo, "支付宝支付","支付宝支付失败...");
            }
            else if (m_pShareInfo->m_enPayMode == enWeChatPay)
            {
                    SetInputContent(m_pShareInfo, "微信支付","微信支付失败...");
            }
            else if (m_pShareInfo->m_enPayMode == enVipPay)
            {
                    SetInputContent(m_pShareInfo, "会员卡支付","会员卡支付失败...");
            }
            else if (m_pShareInfo->m_enPayMode == enBankPay)
            {
                    SetInputContent(m_pShareInfo, "扫码支付","扫码支付失败...");
            }
            m_pShareInfo->UpdateGui();
            // 支付成功，2S后自动开始打印签购单
            usleep(2000*1000);
            return 1;
        }
        else if(enPayRefundSuccess == m_pShareInfo->m_ShareSaleInfo.SaleStatus)
        {
            if (m_pShareInfo->m_enPayMode == enUnionPay)
            {
                    SetInputContent(m_pShareInfo,"银联卡支付", "银联卡支付退款成功...");
            }
            else if (m_pShareInfo->m_enPayMode == enAliPay)
            {
                    SetInputContent(m_pShareInfo, "支付宝支付","支付宝支付退款成功...");
            }
            else if (m_pShareInfo->m_enPayMode == enWeChatPay)
            {
                    SetInputContent(m_pShareInfo, "微信支付","微信支付退款成功...");
            }
            else if (m_pShareInfo->m_enPayMode == enVipPay)
            {
                    SetInputContent(m_pShareInfo, "会员卡支付","会员卡支付退款成功...");
            }
            else if (m_pShareInfo->m_enPayMode == enBankPay)
            {
                    SetInputContent(m_pShareInfo, "扫码支付","扫码支付退款成功...");
            }
            m_pShareInfo->UpdateGui();
            // 支付成功，2S后自动开始打印签购单
            usleep(2000*1000);
            return 1;
        }
        else if(enPayRefundFail == m_pShareInfo->m_ShareSaleInfo.SaleStatus)
        {
            if (m_pShareInfo->m_enPayMode == enUnionPay)
            {
                    SetInputContent(m_pShareInfo,"银联卡支付", "银联卡支付退款失败...");
            }
            else if (m_pShareInfo->m_enPayMode == enAliPay)
            {
                    SetInputContent(m_pShareInfo, "支付宝支付","支付宝支付退款失败...");
            }
            else if (m_pShareInfo->m_enPayMode == enWeChatPay)
            {
                    SetInputContent(m_pShareInfo, "微信支付","微信支付退款失败...");
            }
            else if (m_pShareInfo->m_enPayMode == enVipPay)
            {
                    SetInputContent(m_pShareInfo, "会员卡支付","会员卡支付退款失败...");
            }
            else if (m_pShareInfo->m_enPayMode == enBankPay)
            {
                    SetInputContent(m_pShareInfo, "扫码支付","扫码支付退款失败...");
            }
            m_pShareInfo->UpdateGui();
            // 支付成功，2S后自动开始打印签购单
            usleep(2000*1000);
            return 1;
        }
        else if(enPayVipCardErr == m_pShareInfo->m_ShareSaleInfo.SaleStatus)
        {
            if (m_pShareInfo->m_enPayMode == enVipPay)
            {
                    SetInputContent(m_pShareInfo, "会员卡支付","该卡为无效卡,请重新刷卡...");
            }
            m_pShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
            m_pShareInfo->UpdateGui();
            // 支付成功，2S后自动开始打印签购单
            return 0;
        }
        else if (m_pShareInfo->m_ShareSaleInfo.SaleStatus == enPayQrWait)
        {
                // 支付方式不支持，2S后自动返回
                printf("----------------------SALE_PAY_QR----------------------------------bigen\n");
                m_pShareInfo->m_ShareSaleInfo.SaleStatus = enPayQrSA;
                SetInputContent(m_pShareInfo,"","正在生成二维码...");
                m_pShareInfo->UpdateGui();

                printf("Save QEImage... \n");
                system("rm -rf /root/Application/QRBMP/qrpay.bmp");
                memset(m_pShareInfo->shareInfoPayUi.payQr,0x00,LEN128);
                sprintf(m_pShareInfo->shareInfoPayUi.payQr, "%s", m_pShareInfo->m_ShareSaleInfo.SalePayQr.toStdString().c_str());
                CReceiptThermal qr;
                qr.SaveQrcode((char *)m_pShareInfo->m_ShareSaleInfo.SalePayQr.toStdString().c_str());
                printf("----------------------SALE_PAY_QR----------------------------------end\n");
                return 1;
        }
    }
    return 0;
}

int showInputMsgProc(CShareInfo *m_pShareInfo,enInputType InputType, char *pMSG)
{
    memset(m_passwdCard,0 ,PASSWORDLEN);
    memset(m_pShareInfo->m_stInputInfo.input, 0, LEN32);
    m_pShareInfo->m_enQtUiType = enQt_InputQT;
    m_pShareInfo->m_stInputInfo.cmd = MSG_MESSAGE_REQ;	//请求还是确认消息

    SetInputContent(m_pShareInfo,pMSG,"");

    m_pShareInfo->UpdateGui();

    unsigned char ch;
    unsigned char step=0;
    bool bUpdate = false;
    printf("-------------------------------------------showInput\n");
    while(true)
    {
        usleep(100*1000);

        if (SALE_KEY_VALID == ATKey_getKey(&ch))
        {
            bUpdate = true;
            switch(ch)
            {
                case _KEY_ENTER:
                    if(CheckInputEnter(m_pShareInfo,InputType)>0)
                    {
                        m_pShareInfo->m_stInputInfo.cmd = MSG_MESSAGE_OK;
                        m_pShareInfo->UpdateGui();
                        return 1;
                    }
                   m_pShareInfo->UpdateGui();
                    break;
                case _KEY_CANCEL:
                    m_pShareInfo->m_stInputInfo.cmd = MSG_MESSAGE_OK;
                    m_pShareInfo->UpdateGui();
                    return 0;
                case _KEY_CLR:
                    memset(m_passwdCard,0 ,PASSWORDLEN);
                    memset(m_pShareInfo->m_stInputInfo.input, 0, LEN32);
                    bUpdate = true;
                    break;
                    //下面是输入密码
                case _KEY_NUM0:
                case _KEY_NUM1:
                case _KEY_NUM2:
                case _KEY_NUM3:
                case _KEY_NUM4:
                case _KEY_NUM5:
                case _KEY_NUM6:
                case _KEY_NUM7:
                case _KEY_NUM8:
                case _KEY_NUM9:
                case _KEY_DOT:
                case _KEY_BACK:
                    step = strlen(m_passwdCard);
                    if (step >= PASSWORDLEN-1)
                    {
                            break;
                    }

                    if(ch == _KEY_BACK)
                    {
                        if(step == 0)
                                memset(m_passwdCard, 0, PASSWORDLEN);
                        else
                                m_passwdCard[step-1] = '\0';
                    }
                    else
                    {
                        if(ch == _KEY_DOT)
                        {
                            ch = '.';
                        }
                        else{
                            ch = 0x30 + (ch - _KEY_NUM0);
                        }
                        memcpy(m_passwdCard+step, &ch, 1);

                    }
                    memset(m_pShareInfo->m_stInputInfo.input, 0, LEN32);
                    memcpy(m_pShareInfo->m_stInputInfo.input, m_passwdCard, strlen(m_passwdCard));
                    bUpdate = true;
                    break;

                default:
                        break;
            }
            if (bUpdate)
            {
                   m_pShareInfo->UpdateGui();
                    bUpdate = false;
            }
        }
    }
}

 int CheckInputEnter(CShareInfo *m_pShareInfo,enInputType InputType)
 {
    if(InputType == enInputPasswd)
    {
        // 检测密码是否正确
        if (strcmp(m_passwdCard, "13579") == 0)
        {
                return 1;
        }
        else if (strcmp(m_passwdCard, "314100") == 0)
        {
                m_pShareInfo->m_nMenuFlag = 1;
                return 1;
        }
        else
        {
            SetInputContent(m_pShareInfo,"请重新输入密码","");
            memset(m_passwdCard,0 ,PASSWORDLEN);
            memset(m_pShareInfo->m_stInputInfo.input, 0, LEN32);
            return 0;
        }
    }
    else if(InputType == enInputCalpasswd)
    {
        if(strcmp(m_passwdCard, "314100") == 0)
        {
            return 1;
        }
        else
        {
            SetInputContent(m_pShareInfo,"请重新输入验证码","");
            memset(m_passwdCard,0 ,PASSWORDLEN);
            memset(m_pShareInfo->m_stInputInfo.input, 0, LEN32);
            return 0;
        }
    }
    return 0;
 }


