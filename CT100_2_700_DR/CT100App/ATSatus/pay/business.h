#ifndef BUSINESS_H
#define BUSINESS_H
#include "../../unit/shareinfo.h"
#include "./icbc/icbc.h"

#define	BUSINESS_PROCESS_WAIT                       0x00
#define	BUSINESS_PROCESS_CANCEL                     0x01
#define	BUSINESS_PROCESS_RETURN                     0x02
#define	BUSINESS_PROCESS_PAY_WAIT                   0x03

#define	BUSINESS_PROCESS_POS_U_CONSUME              0x1A
#define	BUSINESS_PROCESS_POS_U_BALANCE              0x1B
#define	BUSINESS_PROCESS_POS_U_TRADING              0x1C
#define	BUSINESS_PROCESS_POS_U_CANCEL               0x1D
#define	BUSINESS_PROCESS_POS_U_REFUND               0x1E

#define	BUSINESS_PROCESS_VIP_CARD_CONSUME           0x2A
#define	BUSINESS_PROCESS_POS_VIP_CARD_BALANCE		0x2B
#define	BUSINESS_PROCESS_POS_VIP_CARD_TRADING		0x2C
#define	BUSINESS_PROCESS_POS_VIP_CARD_CANCEL		0x2D

#define	BUSINESS_PROCESS_ICBC_CONSUME		0x5A
#define	BUSINESS_PROCESS_ICBC_CANCEL		0x5B
#define	BUSINESS_PROCESS_ICBC_REFUND		0x5C
#define	BUSINESS_PROCESS_ICBC_SM            0x5D
#define	BUSINESS_PROCESS_ICBC_PAY_WAIT      0x0E

class CBusiness
{
public:
    CBusiness(CShareInfo *pInfo);
    ~CBusiness(void);
    //各类支付接口 POS 支付宝 微信  三方
    // POS 机接口
    int BusinessPosConsume(void);
    int BusinessPosSwapCancel(void);
    int BusinessPosInquire(void);
    int BusinessPosStop(void);	// 通知停止等待刷卡

    // ICBC银行接口
    int BusinessBankpayPrecreate(void);
    int BusinessBankpayRefund(void);
    int BusinessBankpayPay(void);

    // 会员卡接口
    int BusinessVipCardPay(void);

    // 其他接口
    int BusinessPosOtherProcess(void);	// 其他接口

    CShareInfo 		*m_pInfoBusiness;
    unsigned char		m_nBusinessState;
};

#endif // BUSINESS_H
