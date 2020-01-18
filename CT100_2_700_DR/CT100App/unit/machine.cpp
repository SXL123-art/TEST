#include "machine.h"

CMachine::CMachine(void)
{
    ATPwm_Init();
    ATPmu_Init();
    ATPwm_SetBackLight(8);
    ATAd_initAdc_param();
    ATBeepInit();
    ATLib_Init();
    ATIICInit();
}

CMachine::~CMachine(void)
{

}

int CMachine::initialize()
{
    m_pInfo = new CShareInfo();
    m_pInfo->init();

    m_pLogin = new CLogin(m_pInfo);

    m_pSale = new CMainSale(m_pInfo);

    m_pPay   = new  CPay(m_pInfo);

    m_pMenu = new CMainMenu(m_pInfo);

    m_pCalibration = new Calibration(m_pInfo);

    m_pReport = new CReport(m_pInfo);

    m_pGoodsKey = new CGoodsKey(m_pInfo);

    ATWebServerInit(m_pInfo);

    ScaleHttpServerInit(m_pInfo);


    return 0;
}

void CMachine::run()
{
    printf("-------------------------------------------initialize   start\n");

    initialize();

    printf("-------------------------------------------initialize   complete\n");
    while(true)
    {
        usleep(100*1000);
        printf("-------------------------------------------display change :%d\n",m_pInfo->m_enQtUiType);
        switch(m_pInfo->m_enQtUiType)
        {
            case enQt_LoginQT:   //登录界面只能销售界面 菜单界面 标定界面 转换
                switch(m_pLogin->LoginProc())
                {
                    case enQt_LoginQT:
                        m_pInfo->m_enQtUiType = enQt_LoginQT;
                        break;
                    case enQt_SaleQT:
                        m_pInfo->m_enQtUiType = enQt_SaleQT;
                        break;
                    case enQt_MenuQT:
                        m_pInfo->m_enQtUiType = enQt_MenuQT;
                        break;
                    case enQt_CalQt:
                        m_pInfo->m_enQtUiType = enQt_CalQt;
                        break;
                    default:break;

                }
                m_pInfo->enLastShowType = enQt_LoginQT;
                break;
            case enQt_SaleQT://
                switch(m_pSale->SaleProc())
                {
                    case enQt_LoginQT:
                        m_pInfo->m_enQtUiType = enQt_LoginQT;
                        break;
                    case enQt_SaleQT:
                        m_pInfo->m_enQtUiType = enQt_SaleQT;
                        break;
                    case enQt_PayQT:
                        m_pInfo->m_enQtUiType = enQt_PayQT;
                        break;
                    case enQt_MenuQT:
                        m_pInfo->m_enQtUiType = enQt_MenuQT;
                        break;
                    case enQt_CalQt:
                        m_pInfo->m_enQtUiType = enQt_CalQt;
                        break;
                    case enQt_ReportQt:
                        m_pInfo->m_enQtUiType = enQt_ReportQt;
                        break;
                    case enQt_GoodsKeyQT:
                        m_pInfo->m_enQtUiType = enQt_GoodsKeyQT;
                        break;
                    default:break;
                }
                 m_pInfo->enLastShowType = enQt_SaleQT;
                break;
            case enQt_PayQT:
                switch(m_pPay->PayProc())
                {
                    case enQt_SaleQT:
                        m_pInfo->m_enQtUiType = enQt_SaleQT;
                        break;
                    case enQt_PayQT:
                        m_pInfo->m_enQtUiType = enQt_PayQT;
                        break;
                    default:break;
                }
                 m_pInfo->enLastShowType = enQt_PayQT;
                break;
            case enQt_MenuQT:
                switch(m_pMenu->MainMenuProc())
                {
                    case enQt_LoginQT:
                        m_pInfo->m_enQtUiType = enQt_LoginQT;
                        break;
                    case enQt_SaleQT:
                        m_pInfo->m_enQtUiType = enQt_SaleQT;
                        break;
                    case enQt_MenuQT:
                        m_pInfo->m_enQtUiType = enQt_MenuQT;
                        break;
                    default:break;

                }
                m_pInfo->enLastShowType = enQt_MenuQT;
                break;
            case enQt_CalQt:
                switch(m_pCalibration->CalibrationProc())
                {
                    case enQt_LoginQT:
                        m_pInfo->m_enQtUiType = enQt_LoginQT;
                        break;
                    case enQt_SaleQT:
                        m_pInfo->m_enQtUiType = enQt_SaleQT;
                        break;
                    case enQt_CalQt:
                        m_pInfo->m_enQtUiType = enQt_CalQt;
                        break;
                    default:break;

                }
                m_pInfo->enLastShowType = enQt_CalQt;
                break;
            case enQt_ReportQt:
                switch(m_pReport->ReportProc())
                {
                    case enQt_SaleQT:
                        m_pInfo->m_enQtUiType = enQt_SaleQT;
                        break;
                    case enQt_ReportQt:
                        m_pInfo->m_enQtUiType = enQt_ReportQt;
                        break;
                    default:break;

                }
                m_pInfo->enLastShowType = enQt_ReportQt;
                break;

            case enQt_GoodsKeyQT:
                switch(m_pGoodsKey->GoodsKeyProc())
                {
                    case enQt_SaleQT:
                        m_pInfo->m_enQtUiType = enQt_SaleQT;
                        break;
                    case enQt_GoodsKeyQT:
                        m_pInfo->m_enQtUiType = enQt_GoodsKeyQT;
                        break;
                    default:break;

                }
                m_pInfo->enLastShowType = enQt_GoodsKeyQT;
                break;
        }
    }
}
