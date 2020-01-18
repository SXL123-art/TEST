#ifndef TYPESHOW_H
#define TYPESHOW_H
#include <QString>
//qt显示相关,控制显示
typedef struct
{
    unsigned char dec_pt_TARE;
    unsigned char dec_pt_WEIGHT;
    unsigned char dec_pt_UNIT;
    unsigned char dec_pt_TOTAL;
    unsigned char round;//精确到分，角，元
}decimal;

typedef struct
{
    QString HdVersion;
    QString SwVersion;
    unsigned int MaxWeight;
    unsigned int MinWeight;
    unsigned int ScreenTimeout;
    unsigned int BackLight;// 背光时间，分钟为单位。为0时关闭
    unsigned char Round;				// 四舍五入
    unsigned int Digit;
    unsigned int nWeightCoe;
    unsigned int nPricesCoe;
    decimal m_curDecimal;
}System_Info;
// 网络信息
typedef struct
{
    QString RemoteServer;		// http://192.168.1.2:8080
    QString InternalServer;		// http://192.168.1.2:9090
    QString Mac;					// 64:5A:04:AE:6E:98
    QString Ip;					// 192.168.1.3
    QString Mask;				// 255.255.255.0
    QString Gateway;			// 192.168.1.1
    QString Net_Mode;			// 0
    QString SSID;				// NMSC
    QString PassWord;			// abc12345
    QString Encryption;			// WPA
    QString Dhcp;					// 0
    QString Dns;					// 192.168.1.2/192.168.1.5
} Net_Info;
// 用户自定义参数
typedef struct
{
    unsigned char TotalPriceSwitch;
    unsigned char SavePluSwitch;
    unsigned char SavePriceSwitch;
    unsigned char RecordSwitch;
    unsigned char PrintSwitch;
    float	 fThresholdVale;
} User_Switch;
// 系统开关
typedef struct
{
    unsigned char LoginMode;			// 0:自动登陆；1:账号登陆；2:刷卡登陆
    unsigned char PrintPaper;
    unsigned char CashSwitch;
    unsigned char CameraSwitch;
    unsigned char RfidSwitch;
    unsigned char BluetoothSwitch;
    unsigned char ZigbeeSwitch;
    unsigned char PosSwitch;
    unsigned char ScannerSwitch;
    unsigned char ManageSwitch;
    unsigned char WebSwitch;
    unsigned char AlipaySwitch;
    unsigned char WechatSwitch;
    unsigned char OtherSwitch;
    unsigned char DebugSwitch;
} System_Switch;

// 经营者信息
typedef struct
{
    QString AlipayId;
    QString AlipayNo;
    QString WechatId;
    QString WechatNo;
    QString WechatKey;
    QString OtherID;
    QString OtherNo;
    QString BluetoothName;
    QString BluetoothPsd;
    QString ZigbeeNetID;
    QString ZigbeeChannel;
} Customer_Info;

typedef struct
{
    QString BoothLoginName;
    QString BoothLoginPasswd;
    QString DeviceId;
    QString DeviceType;
    QString BoothId;
    QString BoothName;
    QString BoothCode;
    QString ScaleNum;
    QString MarketCode;
    QString MarketName;
    QString HeadTail1;
    QString HeadTail2;
    QString HeadTail3;
    QString HeadTail4;
    QString HeadTail5;
    QString HeadTail6;
} Device_Info;
// 交易信息
typedef struct
{
    unsigned char SaleId;
    unsigned char SaleType;
    unsigned char SaleAmount;
    unsigned char SaleStatus;
    unsigned char PayType;
    unsigned long PayMoney;
    unsigned int BusinessCode;
    unsigned long RefundMoney;
    QString PhotoTime;
    QString SaleSerial;
    QString SaleClosingTime;
    QString SaleTradeNo;
    QString SalePayQr;
} Sale_Info;

//新济南浪潮 实时获取码
//typedef struct
//{
//    QString PluTraceCode;
//    void ClearAllData(void)
//    {
//        PluTraceCode.clear();
//    }
//} struct_trace_info;

#endif // TYPESHOW_H
