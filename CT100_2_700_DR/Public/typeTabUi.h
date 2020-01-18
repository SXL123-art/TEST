#ifndef TYPETABUI_H
#define TYPETABUI_H
#include <string>
using namespace std;
#include <iostream>
#include <sstream>
#include <QString>

#define PASSWORDLEN	64

#define MAINMENU_SIZE  7
#define MENUNET_SIZE  12
#define MENUUSER_SIZE  14
#define MENUCUSTOMER_SIZE 11
#define MENUSYSTEM_SIZE 5
#define MENUDATA_SIZE 7
#define MENUUSERSWITCH_SIZE 6
#define MENUSYSTEMSWITCH_SIZE 15

#define CAL_SIZE  9
#define REPORT_SIZE 11

// 消息对话框类型
typedef enum enQTSHOWTYPE{
    enQt_LoginQT = 0,
    enQt_SaleQT,
    enQt_PayQT,
    enQt_MenuQT,
    enQt_CalQt,
    enQt_ReportQt,
    enQt_GoodsKeyQT,
    enQt_MsgQT,
    enQt_InputQT,
    enQt_AddSaleQT
}enQtShowType;

#define LEN128 128
#define LEN64  64
#define LEN32  32
#define LEN16  16
#define LEN10 10

enum enADAC{
    enAC = 0,
    enDC,
};

enum enWIFI{
    enWLANT = 0,
    enWLANX,
    enLANT,
    enLANX
};
//销售界面显示内容结构体
typedef struct TabSaleUi
{
    char plu[LEN64];
    char tare[LEN10];
    char weight[LEN10];
    char unitprice[LEN10];
    char totalprice[LEN10];
    char F1[LEN10];
    char F2[LEN10];
    char F3[LEN10];
    char F4[LEN10];
    char ver[LEN10];
    char sellerName[LEN32];
    char systemName[LEN32];
    char message[LEN64];
    char AD[LEN10];
    volatile  char wifi;
    volatile   char ACDC;

    bool ExternalWeb;
    bool InternalWeb;
    bool bluetooth;
    bool cam;
    bool serial;
    bool stable;
    bool net;
    bool zero;
    bool saleMode;

    char downloadUrl[LEN64];
    char picturlUrl[LEN64];
}stTabSaleUi;

typedef struct TabLoginUi
{
    char tare[LEN10];
    char weight[LEN10];
    char user[LEN32];
    char psw[LEN32];
    bool select;
    char stable;
    char net;
    char zero;
    char hdversion[LEN32];
    char swversion[LEN32];
    char message[LEN64];
    float  m_nMaxWeight;
}stTabLoginUi;

typedef struct TabPayUi
{
    char title[LEN32];
    char saleNum[LEN10];
    char payMode[LEN32];
    char payMoney[LEN32];
    char payQr[LEN128];
    char message[LEN64];
}stTabPayUi;

typedef struct TabCalibrationUi
{
    int focus_parent;
    char menuData[LEN16];
    char Statusbar[LEN64];
}stTabCalibrationUi;

typedef struct TabMenuUi
{
    char Statusbar[LEN128];
    int focus_parent;
    int focus_second;
    bool focus_child;
    char menuData[LEN64];
}stTabMenuUi;

#define MSG_MESSAGE_REQ		0x07
#define MSG_MESSAGE_OK		0x08
// message
typedef struct tagMESSAGEQT
{
    char cmd;
    char title[LEN32];
    char message[LEN64];
}stMessageQT;

//input
typedef struct tagINPUTQT
{
    char cmd;
    char message[LEN32];
    char input[LEN32];
}stInputQT;

typedef struct TabAddSaleUi
{
    char cmd;
    char title[LEN64];
    char name[LEN16];
    char weight[LEN16];
    char price[LEN16];
    char total[LEN16];
    char msg[LEN64];
    char discount[LEN32];
}stAddSaleQT;

typedef struct tagSTPLUQT
{
    char plu[LEN64];
    char weight[LEN16];
    char total[LEN16];
    char payMode[LEN16];
}stPluQT;


typedef struct tagREPORTQT
{
    char index;
    int  totalNum;
    int  curNum;
    stPluQT table[REPORT_SIZE];
}stReportQT;
//goodskey
typedef struct tagGOODSKEY
{
    bool cmd;
    int index;
    char goodsName[LEN64];
    char goodsCode[LEN32];
    char goodsKey[LEN10];
    char message[LEN64];
}stGoodsKeyQT;

typedef struct tagMOVIEQT
{
    char 	type;
    bool         update;
    stTabSaleUi saleUi;
    stTabLoginUi loginUi;
    stTabPayUi   payUi;
    stTabMenuUi  menuUi;
    stTabCalibrationUi  CalibrationUi;
    stReportQT    ReportUi;
    stGoodsKeyQT goodsKeyUi;
    stMessageQT  msgUi;
    stInputQT   inputUi;
    stAddSaleQT addSaleUi;
}stShowQT,*pstShowQT;

#define SHAREMEM_LEN 	sizeof(stShowQT)

#endif
