#ifndef TYPEENUM_H
#define TYPEENUM_H


// 消息对话框类型
enum enMsgType{
    enMsgDeviceError = 0,
    enMsgRfid,
    enMsgPay,
    enMsgFx,
    enMsgFn,
    enMsgAlarm,
    enMsgRfidTest
};

// 输入对话框类型
enum enInputType{
    enInputPasswd = 0,
    enInputCalpasswd,
    enInputTelNum,
    enInputAmount
};
enum enSaleMode
{
    enWeight = 0,
    enPieces
};
//支付方式（1现金，3支付宝，4微信，34扫码支付，2银行卡，5会员卡）
enum enPayMode
{
    enNoPay = 0,
    enCash = 1,
    enUnionPay = 2,
    enAliPay = 3,
    enWeChatPay = 4,
    enVipPay = 5,
    enBankPay = 34,
};
enum enPayState{
    enPayWait = 0,
    enPaySuccess,
    enPayTimeOut,
    enPayFail,
    enPayUnSupport,
    enPayPrintReady,
    enPaySave,
    enPayPrintCancel,
    enPayPutUp,
    enPayPutDown,
    enPayComplete,
    enPayCancel,
    enPayQrWait,
    enPayQrSA,
    enPayQrQA,
    enPayRfidCancel,
    enPayVipCardErr,
    enPayRefundSuccess,
    enPayRefundFail
};

#endif // TYPEENUM_H
