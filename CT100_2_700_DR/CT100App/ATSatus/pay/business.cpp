#include "business.h"


void* theadScaleProc(void* p)
{
    int nBusinessRet = 0;
    CBusiness* pBusiness = (CBusiness*)p;
    printf("-------------------------------------------[Proc CBusiness Data]------------------------------\n");

    while(true)
    {
        usleep(100*1000);

        if (pBusiness->m_nBusinessState == BUSINESS_PROCESS_ICBC_CONSUME)
        {
            nBusinessRet = pBusiness->BusinessBankpayPrecreate();
            pBusiness->m_nBusinessState = BUSINESS_PROCESS_WAIT;
            if (nBusinessRet > 0)
            {
                // 支付成功
                pBusiness->m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPaySuccess;
                ATBeep();
                printf("-----------------------SALE_PAY_SUCCESS-------------\n");
            }
            else
            {
                // 支付失败
                pBusiness->m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPayFail;
                ATBeepErr();
                printf("-----------------------SALE_PAY_FAIL:%d-------------\n", nBusinessRet);
            }
        }
        else if (pBusiness->m_nBusinessState == BUSINESS_PROCESS_ICBC_SM)
        {
            printf("-----------------------SALE_PAY_ICBC_SM-------------\n");
            // 处理支付宝扫码支付
            nBusinessRet = pBusiness->BusinessBankpayPay();
            pBusiness->m_nBusinessState = BUSINESS_PROCESS_WAIT;
            if (nBusinessRet > 0)
            {
                // 支付成功
                pBusiness->m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPaySuccess;
                printf("-----------------------SALE_PAY_SUCCESS-------------\n");
            }
            else
            {
                // 支付失败
                pBusiness->m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPayFail;
                printf("-----------------------SALE_PAY_FAIL:%d-------------\n", nBusinessRet);
            }
        }
        else if (pBusiness->m_nBusinessState == BUSINESS_PROCESS_ICBC_REFUND)
        {
            nBusinessRet = pBusiness->BusinessBankpayRefund();
            pBusiness->m_nBusinessState = BUSINESS_PROCESS_WAIT;
            if (nBusinessRet > 0)
            {
                // 退款成功
                pBusiness->m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPayRefundSuccess;
                ATBeep();
                printf("-----------------------BusinessBankpayRefund SUCCESS-------------\n");
            }
            else
            {
                // 退款失败
                pBusiness->m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPayRefundFail;
                ATBeepErr();
                printf("-----------------------BusinessBankpayRefund FAIL:%d-------------\n", nBusinessRet);
            }
        }
//        else if (pBusiness->m_nBusinessState == BUSINESS_PROCESS_ICBC_PAY_WAIT)
//        {

//        }
        else if (pBusiness->m_nBusinessState == BUSINESS_PROCESS_VIP_CARD_CONSUME)
        {
            nBusinessRet = pBusiness->BusinessVipCardPay();
            pBusiness->m_nBusinessState = BUSINESS_PROCESS_WAIT;
            if (nBusinessRet > 0)
            {
                // 支付成功
                pBusiness->m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPaySuccess;
                printf("-----------------------SBusinessVipCardPay SUCCESS-------------\n");
            }
            else
            {
                // 支付失败
                pBusiness->m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPayFail;
                printf("-----------------------BusinessVipCardPay FAIL:%d-------------\n", nBusinessRet);
            }
        }
    }
}

CBusiness::CBusiness(CShareInfo *pInfo)
{
    m_pInfoBusiness = pInfo;

    pthread_t ScaleBusiness;
    pthread_create(&ScaleBusiness, NULL, theadScaleProc, this);
}

int CBusiness::BusinessBankpayPrecreate(void)
{
    bankTradeRequest *request = new bankTradeRequest();
    bankTradeResponse *response = new bankTradeResponse();

    request->app_id = "10000000000000179002";   // APP的编号,应用在API开放平台注册时生成	10000000000000047505
    request->attach = "abcdefg";				// 商户附加数据，最多21个汉字字符，原样返回
    request->mer_id = string((const char *)m_pInfoBusiness->m_ShareCustomerInfo.OtherID.toLocal8Bit());     // 商户线下档案编号410006021347
    request->store_code = string((const char *)m_pInfoBusiness->m_ShareCustomerInfo.OtherNo.toLocal8Bit()); // e生活档案编号	02000015087
    request->tporder_create_ip = string((const char *)m_pInfoBusiness->m_ShareNetInfo.Ip.toLocal8Bit());
    char strTemp[64] = {0};
    sprintf(strTemp, "%d", m_pInfoBusiness->m_ShareSaleInfo.PayMoney);
    request->order_amt = strTemp;			// 扣款金额，分为单位100	表示1.00

    request->out_trade_no = string((const char *)m_pInfoBusiness->m_ShareDeviceInfo.BoothId.toLocal8Bit());
    request->msg_id = "CAS";
    request->msg_id += string((const char *)m_pInfoBusiness->m_ShareDeviceInfo.BoothId.toLocal8Bit());		// 消息通讯唯一编号，每次调用独立生成，APP级唯一
    request->url = "gw.open.icbc.com.cn";		// 正式地址

    int nRet = bankTradePrecreate(request, response);
    if (nRet > 0)
    {
        // 请求订单成功
        m_pInfoBusiness->m_ShareSaleInfo.SalePayQr.clear();
        m_pInfoBusiness->m_ShareSaleInfo.SalePayQr = QString(QString::fromLocal8Bit(response->qr_code.c_str()));
        m_pInfoBusiness->m_ShareSaleInfo.SaleTradeNo.clear();
        m_pInfoBusiness->m_ShareSaleInfo.SaleTradeNo = QString(QString::fromLocal8Bit(request->out_trade_no.c_str()));
        m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPayQrWait;

        int nTimeOut = 0;
        while (1)
        {
            usleep(1000*1000);

            // 取消支付
            if (m_nBusinessState == BUSINESS_PROCESS_ICBC_CANCEL)
            {
                bankTradeCancel(request,response);
                delete(request);
                delete(response);
                nTimeOut = 0;
                return -2;
            }
            // 超时时间1S*120=120
            if (++nTimeOut >= 120)
            {
                // 2分钟超时退出,退出前需要撤销该笔订单
                nTimeOut = 0;
                bankTradeCancel(request,response);
                delete(request);
                delete(response);
                return -1;
            }
            // 查询订单
            nRet = bankTradeQuery(request,response);
            if(nRet>0)
            {
                //记录上一次交易退款金额
                m_pInfoBusiness->m_ShareSaleInfo.RefundMoney = atol(request->order_amt.c_str());
                delete(request);
                delete(response);
                nTimeOut = 0;
                return 1;
            }
        }
    }
    delete(request);
    delete(response);
    return 0;
}
int CBusiness::BusinessBankpayPay(void)
{
    int nLen = 0;
    char Recv[64] = {0};
//    if (m_pInfoBusiness->m_ShareSysSwitchInfo.ScannerSwitch == 1)
//    {
//        if (ATSerialDevice_Scanner_Recv(Recv, nLen) < 0)
//                return -1;
//    }
   if (m_pInfoBusiness->m_ShareSysSwitchInfo.ScannerSwitch == 2)
    {
        while(1)
        {
            usleep(20*1000);
            //if (ATPeripherals_getScanner((unsigned char * )Recv) > 0)
            {
                printf("ATPeripherals_getScanner: %s\n", Recv);
                if (strlen(Recv) == 18)
                {
                    nLen = 1;
                    break;
                }
            }
            if (m_nBusinessState == BUSINESS_PROCESS_ICBC_CANCEL)
                    break;
        }
        if (nLen == 0)
                return -1;
    }
    else
    {
        //m_pInfoBusiness->SetInputContent(enSaleTitle, "请检查扫描枪设置");
        return -1;
    }

    bankTradeRequest *request = new bankTradeRequest();
    bankTradeResponse *response = new bankTradeResponse();

    request->app_id = "10000000000000179002";   // APP的编号,应用在API开放平台注册时生成	10000000000000047505
    request->attach = "abcdefg";                // 商户附加数据，最多21个汉字字符，原样返回
    request->mer_id = string((const char *)m_pInfoBusiness->m_ShareCustomerInfo.OtherID.toLocal8Bit());		// 商户线下档案编号410006021347

    request->auth_code = Recv;	// 付款码
    printf("request->auth_code: %s\n", request->auth_code.c_str());

    char strTemp[64] = {0};
    sprintf(strTemp, "%d", m_pInfoBusiness->m_ShareSaleInfo.PayMoney);
    request->order_amt = strTemp;			// 扣款金额，分为单位100	表示1.00  

    request->out_trade_no = string((const char *)m_pInfoBusiness->m_ShareDeviceInfo.BoothId.toLocal8Bit());
    request->msg_id = "CAS";
    request->msg_id += string((const char *)m_pInfoBusiness->m_ShareDeviceInfo.BoothId.toLocal8Bit());		// 消息通讯唯一编号，每次调用独立生成，APP级唯一
    request->url = "gw.open.icbc.com.cn";		// 正式地址

    int nRet = bankTradePay(request, response);
    if (nRet == 0 || nRet == 1)
    {
        // 扫码支付成功 -1:下单失败，0：支付中，1：支付成功，2：支付失败
        m_pInfoBusiness->m_ShareSaleInfo.SaleTradeNo.clear();
        m_pInfoBusiness->m_ShareSaleInfo.SaleTradeNo = QString(QString::fromLocal8Bit(request->out_trade_no.c_str()));
        if(nRet == 1)
        {
            //记录上一次交易退款金额
            m_pInfoBusiness->m_ShareSaleInfo.RefundMoney = atol(request->order_amt.c_str());
            delete(request);
            delete(response);
            return 1;
        }
        int nTimeOut = 0;
        while (1)
        {
            usleep(500*1000);
            // 取消支付
            if (m_nBusinessState == BUSINESS_PROCESS_ICBC_CANCEL)
            {
                bankTradeCancel(request,response);
                delete(request);
                delete(response);
                nTimeOut = 0;
                return -2;
            }
            // 超时时间1S*120=120
            if (++nTimeOut >= 120)
            {
                // 2分钟超时退出,退出前需要撤销该笔订单
                nTimeOut = 0;
                bankTradeCancel(request,response);
                delete(request);
                delete(response);
                return -1;
            }
            // 查询订单
            nRet = bankTradeQuery(request,response);
            if(nRet>0)
            {
                //记录上一次交易退款金额
                m_pInfoBusiness->m_ShareSaleInfo.RefundMoney = atol(request->order_amt.c_str());
                delete(request);
                delete(response);
                nTimeOut = 0;
                return 1;
            }
        }
    }
    delete(request);
    delete(response);
    return 0;
}
int CBusiness::BusinessBankpayRefund(void)
{
    bankTradeRequest *request = new bankTradeRequest();
    bankTradeResponse *response = new bankTradeResponse();

    request->app_id = "10000000000000179002";   // APP的编号,应用在API开放平台注册时生成	10000000000000047505
    request->attach = "abcdefg";                // 商户附加数据，最多21个汉字字符，原样返回
    request->mer_id = string((const char *)m_pInfoBusiness->m_ShareCustomerInfo.OtherID.toLocal8Bit());

    char strTemp[64] = {0};
    sprintf(strTemp, "%d", m_pInfoBusiness->m_ShareSaleInfo.RefundMoney);
    request->reject_amt = strTemp;										// 退款金额
    request->out_trade_no = string((const char *)m_pInfoBusiness->m_ShareSaleInfo.SaleTradeNo.toLocal8Bit());//m_pInfoBusiness->m_ShareSaleInfo.SaleRefundOrder;		// 原订单号

    request->reject_no = string((const char *)m_pInfoBusiness->m_ShareDeviceInfo.BoothId.toLocal8Bit());
    request->msg_id = "CAS";
    request->msg_id += string((const char *)m_pInfoBusiness->m_ShareDeviceInfo.BoothId.toLocal8Bit());		// 消息通讯唯一编号，每次调用独立生成，APP级唯一
    request->url = "gw.open.icbc.com.cn";		// 正式地址

    int nRet = bankTradeRefund(request,response);
    if (nRet > 0)
    {
            stReceiptData data;
            char str1[64] = {0};
            char str[64] = {0};

            data.type = enBigString;
            sprintf(str1,"%s",m_pInfoBusiness->m_ShareDeviceInfo.MarketName.toStdString().c_str());
            ATLib_u2g(str1,strlen(str1),str,64);
            PrintBigCenter(str, data.str);
            m_pInfoBusiness->m_pPrint->add(data);

            data.type = enCenterString;
            sprintf(str,"%s", "退款凭证");
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pInfoBusiness->m_pPrint->add(data);

            data.type = enString;
            sprintf(data.str,"%s","--------------------------------");
            m_pInfoBusiness->m_pPrint->add(data);

            sprintf(str,"摊位号: %s ", m_pInfoBusiness->m_ShareDeviceInfo.BoothId.toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pInfoBusiness->m_pPrint->add(data);

            sprintf(str,"退款金额: %d.%02d元", m_pInfoBusiness->m_ShareSaleInfo.RefundMoney/100, m_pInfoBusiness->m_ShareSaleInfo.RefundMoney%100);
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pInfoBusiness->m_pPrint->add(data);

            sprintf(str,"商户编号: %s", m_pInfoBusiness->m_ShareCustomerInfo.OtherID.toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pInfoBusiness->m_pPrint->add(data);;

            sprintf(str,"原支付订单: %s", m_pInfoBusiness->m_ShareSaleInfo.SaleTradeNo.toStdString().c_str());
            ATLib_u2g(str,strlen(str),data.str,64);
            m_pInfoBusiness->m_pPrint->add(data);

            data.type = enString;
            sprintf(data.str,"%s","--------------------------------");
            m_pInfoBusiness->m_pPrint->add(data);

            //data.type = enNull;
            //m_pInfoBusiness->m_pPrint->add(data);

            data.type = enStartPrint;
            m_pInfoBusiness->m_pPrint->Print();

            delete(request);
            delete(response);
            return 1;
    }
    delete(request);
    delete(response);
    return -1;
}

int CBusiness::BusinessVipCardPay(void)
{
    char Recv[64] = {0};
    if (m_pInfoBusiness->m_ShareSysSwitchInfo.ScannerSwitch == 2)
    {
        //ATPeripherals_getScanner((unsigned char * )Recv);
        while(1)
        {
            usleep(200*1000);
            //if (ATPeripherals_getScanner((unsigned char * )Recv) > 0)
            {
                printf("ATPeripherals_getScanner: %s\n", Recv);
                if (strlen(Recv) == 10 && strcmp(Recv, "0977615490")==0)
                {
                    // 测试会员卡支付成功
                    m_pInfoBusiness->m_ShareSaleInfo.SaleTradeNo.clear();
                    m_pInfoBusiness->m_ShareSaleInfo.SaleTradeNo = QString(QLatin1String(Recv));
                    return 1;
                }
                else
                {
                    m_pInfoBusiness->m_ShareSaleInfo.SaleStatus = enPayVipCardErr;
                }
            }
            if (m_nBusinessState == BUSINESS_PROCESS_POS_VIP_CARD_CANCEL)
                   break;
        }
    }
    return 0;
}

int CBusiness::BusinessPosOtherProcess(void)
{/*
    if (m_nBusinessState == BUSINESS_PROCESS_ICBC_REFUND)
    {
        if(BusinessBankpayRefund()>0)
        {
            m_pInfoBusiness->SetInputContent(enSaleTitle, "退款成功");
        }
        else
        {
            m_pInfoBusiness->SetInputContent(enSaleTitle, "退款失败");
        }
        usleep(2000*1000);
        m_pInfoBusiness->SetInputContent(enSaleTitle, "称重");
        m_nBusinessState = BUSINESS_PROCESS_WAIT;
    }*/
}
