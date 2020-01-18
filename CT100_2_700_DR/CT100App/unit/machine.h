#pragma once
#ifndef MACHINE_H
#define MACHINE_H

#include "shareInfo.h"
#include "../ATPwm/ATPwm.h"
#include "../ATPmu/ATPmu.h"
#include "../ATScaleHttpServer/ATScaleHttpServer.h"
#include "../ATWebservice/ATWebService.h"
#include "../ATSatus/sale/mainSale.h"
#include "../ATSatus/login/login.h"
#include "../ATSatus/menu/mainmenu.h"
#include "../ATSatus/pay/pay.h"
#include "../ATSatus/calibration/calibration.h"

#include "../ATSatus/report/report.h"
#include "../ATSatus/goodskey/goodskey.h"

class CMachine
{
public:
        CMachine(void);
        ~CMachine(void);

        int initialize();
        void run();

private:
        CLogin*		m_pLogin;		//登陆界面
        CMainSale*	m_pSale;		//销售界面
        CPay*		m_pPay;                 //付款界面
        CMainMenu*	m_pMenu;		//配置界面
        Calibration*    m_pCalibration;         //一键标定界面

        CReport*        m_pReport;              //报表管理
        CGoodsKey*      m_pGoodsKey;            //快捷键对应

        CShareInfo*	m_pInfo;		//刷新类对象
};

#endif
