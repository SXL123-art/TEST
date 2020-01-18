#pragma once
#ifndef SHAREINFO_H
#define SHAREINFO_H
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QByteArray>

#include <math.h>
#include <errno.h>

#include "../ATAdc/ATAdc.h"
#include "../ATBeep/ATBeep.h"
#include "../ATKeyboard/ATKey.h"
#include "../ATPwm/ATPwm.h"
#include "../ATPmu/ATPmu.h"
#include "../ATPrint/ReceiptThermal.h"
#include "../ATI2CDevice/ATI2CDevice.h"
#include "../ATLib/ATLib.h"
#include "../ATSqlite/ATSQLite.h"
#include "../ATI2cDevice/ATI2CDevice.h"
#include "../ATSerialDevice/ATSerialDevice.h"
#include "../ATRtc/ATRtc.h"
#include "../ATShm/shmcom.h"
#include "../ATAtmel8/Atmel8.h"
#include "../public/typeEnum.h"
#include "../../Public/typeTabUi.h" //该头文件两个程序共用，放全局变量和宏

// 键盘类型开关
#define SMALL_KEY    0
#define MIDDLE_KEY   1
#define KEY_TYPE     SMALL_KEY

#define SOFTWARE_VER  "CV2.8"
#define HARDWARE_VER  "CT100_700_V3.2"

//共享信息类
class CShareInfo
{
public:
        CShareInfo(void);
        ~CShareInfo(void);

        int init();
        int UpdateGui(void);
        int RefreshWeight(void);

        enSaleMode GetSaleMode(){return m_enSaleMode;};
        void SetSaleMode(enSaleMode mode){m_enSaleMode=mode;};

        enPayMode GetPayMode(){return m_enPayMode;};
        void SetPayMode(enPayMode mode){m_enPayMode=mode;};

        char KeyValueConversion(unsigned char ch);
        int PerformSqlFile(char *filepath);

        void SystemTimeInit(void);
        int StartNetwork(void);

        int ZeroTare(unsigned char key);
        int ShowScreenBackLight(void);
        int ShowScreenSaveProc(void);
        int ShowOverWeightWarningProc(enMsgType MsgType, char *pTitle, char *pMessage);
        int ShowMessage(char *pMessage);
        int ReadS70CardInfo(void);
        int ReadCPUCardInfo(void);

        int DackTime(char *startTime , char *backTime , int backTimesDay ,bool flag);
        int DeteleSaleInfo();
        int DeteleBatchInfo();
        int Rfid_Find(void);

public:
        //与QT通讯的全局变量
        stTabSaleUi     shareInfoSaleUi;
        stTabLoginUi    shareInfoLoginUi;
        stTabPayUi      shareInfoPayUi;
        stTabMenuUi     shareInfoMenuUi;
        stTabCalibrationUi shareInfoCalibrationUi;

        stMessageQT     m_stMessageInfo;
        stInputQT       m_stInputInfo;
        stAddSaleQT     m_stAddSaleInfo;

        stReportQT         shareReportUi;
        stGoodsKeyQT    m_stGoodsKeyInfo;

        //与数据库交互的变量指针
        struct_tb_product  *stp;
        struct_tb_attach   *sta;
        struct_tb_business *stb;

        //菜单选项的全局变量
        Net_Info        m_ShareNetInfo;
        System_Info     m_ShareSystemInfo;
        User_Switch     m_ShareUserSwitchInfo;
        System_Switch   m_ShareSysSwitchInfo;
        Customer_Info   m_ShareCustomerInfo;
        Device_Info     m_ShareDeviceInfo;
        Sale_Info       m_ShareSaleInfo;

        //AD获取重量的全局变量
        adc_ret		m_adInfo; //存放本次采集的AD值
        adc_ret		m_LastAdInfo;//存放上次采集的AD值

        //共享内存通讯
        shmCom          *m_pShm;//共享内存类，用于QT通讯
        //打印机对象指针
        CReceiptThermal  *m_pPrint;
        //arm与Atmel8通讯AD处理类
        CAtmSerial  *m_pAtmel8;
        //界面切换变量
        enQtShowType    m_enQtUiType;
        enQtShowType    enLastShowType;

        //
        enSaleMode      m_enSaleMode;
        enPayMode       m_enPayMode;
        //
        bool            m_sureUpdateQt;
        unsigned char  m_WebserviceUpFlag;

        enMsgType       m_enMsgType;
        //打印机测试，打印间隔
        long m_nPrintTestTime;
        //重量是否改变
        unsigned char m_bWeightChange;
        unsigned char  m_bSaleKeyChange;
        //
        bool m_nMenuFlag;

};

#endif
