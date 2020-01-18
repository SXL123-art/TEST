#include "pay.h"


CPay::CPay(CShareInfo *pInfo)
{
    m_pPayShareInfo = pInfo;
    m_bUpdate = false;
    m_pBusiness = new CBusiness(m_pPayShareInfo);
}

CPay::~CPay(void)
{

}

enQtShowType CPay::PayProc()
{
    printf("-------------------------------------------CPay\n");

    // 默认现金模式
    ChangePayMode(enCash);
    sprintf(m_pPayShareInfo->shareInfoPayUi.title,"%s","请选择支付方式");
    sprintf(m_pPayShareInfo->shareInfoPayUi.payQr, "%s", "");
    sprintf(m_pPayShareInfo->shareInfoPayUi.payMoney, "%d.%02d", m_pPayShareInfo->m_ShareSaleInfo.PayMoney/100, m_pPayShareInfo->m_ShareSaleInfo.PayMoney%100);
    sprintf(m_pPayShareInfo->shareInfoPayUi.saleNum, "%d", m_pPayShareInfo->m_ShareSaleInfo.SaleAmount);
    printf("m_pPayShareInfo->shareInfoPayUi.saleNum:%d", m_pPayShareInfo->shareInfoPayUi.saleNum);
    m_pPayShareInfo->m_enQtUiType = enQt_PayQT;
    m_pPayShareInfo->UpdateGui();

    while(true)
    {
        usleep(100*1000);
        switch (PayDeal())
        {
        case enQt_SaleQT:
            // 销售界面
            return enQt_SaleQT;
        case enQt_PayQT:
            // 付款界面
            break;
        default:
            break;
        }
        UpdatePayShow();
    }
}
int CPay::CheckPayStatus(void)
{
    if (m_pPayShareInfo->m_ShareSaleInfo.SaleStatus == enPaySuccess)
    {
        sprintf(m_pPayShareInfo->shareInfoPayUi.title,"%s","请选择支付方式");
        printf("--------------------pay success --------------------\n");
        ClearPayInfo();
        m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayPrintReady;
        return 1;
    }
    else if (m_pPayShareInfo->m_ShareSaleInfo.SaleStatus == enPayFail)
    {
        sprintf(m_pPayShareInfo->shareInfoPayUi.title,"%s","请选择支付方式");
        printf("--------------------pay fail --------------------\n");
        m_bUpdate = true;
        memset(m_pPayShareInfo->shareInfoPayUi.payQr,0x00,LEN128);
        usleep(1000*1000);
        // 支付失败，自动跳转到现金支付
        ChangePayMode(enCash);
        m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
    }
    else if (m_pPayShareInfo->m_ShareSaleInfo.SaleStatus == enPayCancel)
    {
        sprintf(m_pPayShareInfo->shareInfoPayUi.title,"%s","请选择支付方式");
        // 支付取消，自动跳转到现金支付
        printf("--------------------pay cancel --------------------\n");
        if (m_pPayShareInfo->m_enPayMode == enUnionPay)
        {
        }
        else if (m_pPayShareInfo->m_enPayMode == enAliPay)
        {
        }
        else if (m_pPayShareInfo->m_enPayMode == enWeChatPay)
        {
        }
        else if (m_pPayShareInfo->m_enPayMode == enVipPay)
        {
            m_pBusiness->m_nBusinessState = BUSINESS_PROCESS_POS_VIP_CARD_CANCEL;
        }
        else if (m_pPayShareInfo->m_enPayMode == enBankPay)
        {
            m_pBusiness->m_nBusinessState = BUSINESS_PROCESS_ICBC_CANCEL;
        }

        ChangePayMode(enCash);
        m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
    }
    else if (m_pPayShareInfo->m_ShareSaleInfo.SaleStatus == enPayQrSA)
    {
        printf("--------------------pay SALE_PAY_SA----------------\n");
        m_bUpdate = true;
        sprintf(m_pPayShareInfo->shareInfoPayUi.title,"%s","请扫码完成支付");
        m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayQrQA;
    }
    else if (m_pPayShareInfo->m_ShareSaleInfo.SaleStatus == enPayRefundSuccess || m_pPayShareInfo->m_ShareSaleInfo.SaleStatus == enPayRefundFail)
    {
        printf("--------------------pay SALE_PAY_REFUND----------------\n");
        sprintf(m_pPayShareInfo->shareInfoPayUi.title,"%s","请选择支付方式");
        m_bUpdate = true;
        ChangePayMode(enCash);
        m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
    }
    return 0;
}
int CPay::UpdatePayShow(void)
{
    if (m_bUpdate)
    {
        m_bUpdate = false;
        m_pPayShareInfo->UpdateGui();
    }
}
int CPay::showMsg(enMsgType MsgType, char *pTitle,char *pMSG)
{
    int nRet = showMsgProc(m_pPayShareInfo,MsgType,pTitle,pMSG);
    printf("-------------------------------------------CPay\n");
    m_pPayShareInfo->m_enQtUiType = enQt_PayQT;
    m_pPayShareInfo->UpdateGui();
    return nRet;
}
int CPay::showInputMsg(enInputType InputType, char *pInputMSG)
{
    int nRet = showInputMsgProc(m_pPayShareInfo,InputType,pInputMSG);
    sprintf(m_pPayShareInfo->m_stInputInfo.input,"%s","");
    printf("-------------------------------------------CPay\n");
    m_pPayShareInfo->m_enQtUiType = enQt_PayQT;
    m_pPayShareInfo->UpdateGui();
    return nRet;
}
enQtShowType CPay::PayDeal()
{
    if (CheckPayStatus() > 0)
    {
        // 刷卡支付完成，自动打印小票，然后退出支付界面
        return enQt_SaleQT;
    }
    unsigned char ch = 0;
    usleep(100*1000);
    if (SALE_KEY_VALID == ATKey_getKey(&ch))
    {
       m_bUpdate = true;
       switch(ch)
       {
       case _KEY_NUM1:
           if(m_pPayShareInfo->GetPayMode() != enCash)
           {
               ATBeepErr();
               break;
           }
           ChangePayMode(enBankPay);
           m_pBusiness->m_nBusinessState = BUSINESS_PROCESS_ICBC_SM;
           m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
           showMsg(enMsgPay,"扫码支付","请使用扫码器扫描手机支付宝或微信付款码..");
           break;
       case _KEY_NUM2:
           if(m_pPayShareInfo->GetPayMode() != enCash)
           {
               ATBeepErr();
               break;
           }
           ChangePayMode(enBankPay);
           m_pBusiness->m_nBusinessState = BUSINESS_PROCESS_ICBC_CONSUME;
           m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
           showMsg(enMsgPay,"扫码支付","请使用手机微信或支付宝扫描屏幕二维码...");
           break;
       case _KEY_NUM3:
           if(m_pPayShareInfo->GetPayMode() != enCash)
           {
               ATBeepErr();
               break;
           }
           // 会员卡
           ChangePayMode(enVipPay);
           m_pBusiness->m_nBusinessState = BUSINESS_PROCESS_VIP_CARD_CONSUME;
           m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
           showMsg(enMsgPay,"会员卡支付","请在扫描器上面刷会员卡...");
           break;
       case _KEY_PLU60:
           if(m_pPayShareInfo->GetPayMode() != enCash)
           {
               ATBeepErr();
               break;
           }
           ChangePayMode(enBankPay);
           m_pBusiness->m_nBusinessState = BUSINESS_PROCESS_ICBC_REFUND;
           m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
           showMsg(enMsgPay,"退款","正在退款中...");

           // 退款
           break;
            /*
            case _KEY_NUM1://银联POS
            if(0 == m_pPayShareInfo->m_ShareSysSwitchInfo.PosSwitch)
            {
                m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayUnSupport;
                showMsg(enMsgPay,"银联POS机支付","无POS或者功能未开通");
                break;
            }
            break;
            case _KEY_NUM2://三方支付
               if(0 == m_pPayShareInfo->m_ShareSysSwitchInfo.OtherSwitch)
               {
                   m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayUnSupport;
                   showMsg(enMsgPay,"银联POS机支付","无POS或者功能未开通");
                   break;
               }
               if(1 == m_pPayShareInfo->m_ShareSysSwitchInfo.OtherSwitch)
               {
                   ChangePayMode(enBankPay);
                   m_pBusiness->m_nBusinessState = BUSINESS_PROCESS_ICBC_CONSUME;
                   m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
                   showMsg(enMsgPay,"扫码支付","发起扫码支付...");
                   break;
               }
               break;
            case _KEY_NUM3://支付宝
               if(0 == m_pPayShareInfo->m_ShareSysSwitchInfo.AlipaySwitch)
               {
                   m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayUnSupport;
                   showMsg(enMsgPay,"支付宝支付","支付宝功能未开通");
                   break;
               }
                break;
            case _KEY_NUM4://微信
               if(0 == m_pPayShareInfo->m_ShareSysSwitchInfo.AlipaySwitch)
               {
                   m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayUnSupport;
                   showMsg(enMsgPay,"微信支付","微信功能未开通");
                   break;
               }
                break;
            case _KEY_NUM5://扫描器
               if(0 == m_pPayShareInfo->m_ShareSysSwitchInfo.OtherSwitch)
               {
                   m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayUnSupport;
                   showMsg(enMsgPay,"银联POS机支付","无POS或者功能未开通");
                   break;
               }
               if(1 == m_pPayShareInfo->m_ShareSysSwitchInfo.OtherSwitch)
               {
                   ChangePayMode(enBankPay);
                   m_pBusiness->m_nBusinessState = BUSINESS_PROCESS_ICBC_SM;
                   m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayWait;
                   showMsg(enMsgPay,"扫码支付","发起扫描器支付...");
                   break;
               }
                break;
          */
           case  _KEY_CANCEL:
               if(m_pPayShareInfo->m_ShareSaleInfo.SaleStatus == enPayQrQA)
               {
                   memset(m_pPayShareInfo->shareInfoPayUi.payQr,0x00,LEN128);
                   m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayCancel;
                   break;
               }
               ClearPayInfo();
               return enQt_SaleQT;
           case _KEY_ENTER:
               if(m_pPayShareInfo->m_enPayMode == enCash)
               {
                   ClearPayInfo();
                   m_pPayShareInfo->m_ShareSaleInfo.SaleStatus = enPayPrintReady;
                   return enQt_SaleQT;
               }
               else
               {
                    break;
               }
                break;
       default:
               break;

       }
    }
    return enQt_PayQT;
}

int CPay::ClearPayInfo(void)
{
    sprintf(m_pPayShareInfo->shareInfoPayUi.title,"%s","请选择支付方式");
    sprintf(m_pPayShareInfo->shareInfoPayUi.payQr, "%s", "");
    sprintf(m_pPayShareInfo->shareInfoPayUi.payMoney, "%s", "0");
    sprintf(m_pPayShareInfo->shareInfoPayUi.saleNum, "%s", "0");
    //ChangePayMode(enCash);
    return 0;
}

void CPay::ChangePayMode(enPayMode mode)
{
    m_pPayShareInfo->SetPayMode(mode);
    switch (mode)
    {
    case enNoPay:
            sprintf(m_pPayShareInfo->shareInfoPayUi.payMode, "%s", "赊账支付");
            break;
    case enCash:
            sprintf(m_pPayShareInfo->shareInfoPayUi.payMode, "%s", "现金支付");
            break;
    case enUnionPay:
            sprintf(m_pPayShareInfo->shareInfoPayUi.payMode, "%s", "银联卡支付");
            break;
    case enAliPay:
            sprintf(m_pPayShareInfo->shareInfoPayUi.payMode, "%s", "支付宝支付");
            break;
    case enWeChatPay:
            sprintf(m_pPayShareInfo->shareInfoPayUi.payMode, "%s", "微信支付");
            break;
    case enVipPay:
            sprintf(m_pPayShareInfo->shareInfoPayUi.payMode, "%s", "会员卡支付");
            break;
    case enBankPay:
            sprintf(m_pPayShareInfo->shareInfoPayUi.payMode, "%s", "扫码支付");
            break;
    default:
            sprintf(m_pPayShareInfo->shareInfoPayUi.payMode, "%s", "现金支付");
            break;
    }
    m_pPayShareInfo->UpdateGui();
}
