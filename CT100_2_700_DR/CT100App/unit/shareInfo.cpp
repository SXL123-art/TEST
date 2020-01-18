#include "shareInfo.h"

#define SWITCH_CHECK_NETWORK		1
#define SWITCH_CHECK_SCREEN		1
#define SWITCH_CHECH_OVERWEIGHT         1

void *theadSystemCheckout(void *p)
{
    CShareInfo* pSystemCheck = (CShareInfo*)p;
    static unsigned char nPingTime = 0;
    static unsigned char nLowDcCnt = 0;
    static bool bIsCkeckAc = false;
    while(1)
    {
        usleep(1000*1000);

        if(pSystemCheck->m_enQtUiType != enQt_SaleQT)		//in sale windows
        {
            continue;
        }


        if(nLowDcCnt++ >= 20)
        {
            nLowDcCnt = 0;
            if( pSystemCheck->m_pAtmel8->m_nBatteryVol <=15 && pSystemCheck->m_pAtmel8->m_bAc < 0)
            {
                bIsCkeckAc = true;
                ATBeep();
                pSystemCheck->ShowMessage("电量过低,即将关机,请插入电源");
            }
        }
        else
        {
            if(bIsCkeckAc && pSystemCheck->m_pAtmel8->m_bAc > 0 )
            {
                bIsCkeckAc = false;
                pSystemCheck->ShowMessage("电源已插入,正在充电...");
            }
        }
        if(pSystemCheck->m_pAtmel8->m_bAc > 0)
        {
            pSystemCheck->shareInfoSaleUi.ACDC = enAC;
            sprintf(pSystemCheck->shareInfoSaleUi.AD,"%d",pSystemCheck->m_pAtmel8->m_nBatteryVol);
            //printf("ac-ad:%d\n",pSystemCheck->m_pAtmel8->m_nBatteryVol);

        }
        else
        {
            pSystemCheck->shareInfoSaleUi.ACDC = enDC;
            sprintf(pSystemCheck->shareInfoSaleUi.AD,"%d",pSystemCheck->m_pAtmel8->m_nBatteryVol);

            //printf("dc-ad:%d\n",pSystemCheck->m_pAtmel8->m_nBatteryVol);
        }
        if(pSystemCheck->m_pAtmel8->m_bCheckSerial == false)
        {
            pSystemCheck->shareInfoSaleUi.serial = 0;
        }
        else
        {
            pSystemCheck->shareInfoSaleUi.serial = 1;
        }

        pSystemCheck->UpdateGui();
        if (SWITCH_CHECK_NETWORK)
        {
            if (pSystemCheck->shareInfoSaleUi.ExternalWeb)
            {
                // 服务器连接正常，不需要ping内网
                if (pSystemCheck->m_ShareNetInfo.Net_Mode.compare("1") == 0)
                {
                    pSystemCheck->shareInfoSaleUi.wifi  = enWLANT;
                }
                else
                {
                    pSystemCheck->shareInfoSaleUi.wifi  = enLANT;
                }

                continue;
            }
            // 网络检测，500*200=60000,一分钟ping一次
            if (++nPingTime >= 60)
            {
                nPingTime = 0;
                char strSystem[32] = {0};
                //pSystemCheck->m_ShareNetInfo.Gateway = "1.194.237.140";   // scaleapi.hunyuantech.com
                sprintf(strSystem, "ping %s -c 1", pSystemCheck->m_ShareNetInfo.Gateway.toStdString().c_str());
                if ( system(strSystem) != 0)
                {
                    if (pSystemCheck->m_ShareNetInfo.Net_Mode.compare("1") == 0)
                    {
                        pSystemCheck->shareInfoSaleUi.wifi  = enWLANX;
                    }
                    else
                    {
                        pSystemCheck->shareInfoSaleUi.wifi  = enLANX;
                    }
                    /*
                    if(pSystemCheck->m_ShareNetInfo.Dhcp.compare("1") == 0)
                    {

                    }
                    */

                    pSystemCheck->StartNetwork();
                }
                else
                {
                    if (pSystemCheck->m_ShareNetInfo.Net_Mode.compare("1") == 0)
                    {
                        pSystemCheck->shareInfoSaleUi.wifi  = enWLANT;
                    }
                    else
                    {
                        pSystemCheck->shareInfoSaleUi.wifi  = enLANT;
                    }
                }


            }
        }

    }
}

void* theadOverBackLight(void* p)
{
    CShareInfo* pSystemCheck = (CShareInfo*)p;
    static unsigned int nBackLightTime = 0;
    static bool bBackLight = true;
    printf("-------------------------------------------[theadOverBackLight]--------------------\n");
    while(1)
    {
        usleep(100*1000);    // 50ms时间不能延长，否则重量变化检测不出来
        if(pSystemCheck->m_ShareSystemInfo.ScreenTimeout <= 0)
        {
            usleep(1000*1000);
            nBackLightTime = 0;
            continue;
        }

        //printf("---------------------------Lastkg:%ld kg:%ld m_bKeySure:%d\n",pSystemCheck->m_LastAdInfo.kg,pSystemCheck->m_adInfo.kg,pSystemCheck->m_pStm32AndRAM->m_bKeySure);
        if ((pSystemCheck->m_bWeightChange==2) || 1)//(pSystemCheck->m_pAtmel8->m_bKeySure)
        {
            if(pSystemCheck->m_bWeightChange==2)pSystemCheck->m_bWeightChange = 0;

            nBackLightTime = 0;
            //printf("1--------------------------- bBackLight:%d\n", bBackLight);
            if(!bBackLight)
            {
                bBackLight = true;
                ATPwm_SetBackLight(8);
            }
        }
        else
        {
            if(pSystemCheck->m_enQtUiType != enQt_SaleQT)
            {
                usleep(1000*1000);
                nBackLightTime = 0;
                continue;
            }
            if ((++nBackLightTime >= pSystemCheck->m_ShareSystemInfo.ScreenTimeout * 20))
            {
                //printf("2--------------------------- bBackLight:%d\n", bBackLight);
                nBackLightTime = 0;
                if(bBackLight)
                {
                    bBackLight = false;
                    ATPwm_SetBackLight(0);
                }
            }
        }
    }
}
void* theadAD(void* p)
{
    CShareInfo* pus = (CShareInfo*)p;
    //得到重量
    while(1)
    {
        usleep(80*1000);

        if(pus->m_enQtUiType == enQt_SaleQT || pus->m_enQtUiType == enQt_LoginQT
                || pus->m_enQtUiType == enQt_AddSaleQT || pus->m_enQtUiType == enQt_MsgQT)
        {
            ATAd_getKg(&(pus->m_adInfo));
        }
    }
}

CShareInfo::CShareInfo()
{

    m_enSaleMode = enWeight;		// 默认计重模式
    m_enQtUiType = enQt_SaleQT;
    SetPayMode(enCash);                 // 默认现金交易
    m_sureUpdateQt = false;
    m_nMenuFlag = false ;
    m_WebserviceUpFlag = 0;            //web状态

    m_ShareSystemInfo.SwVersion = SOFTWARE_VER;
    m_ShareSystemInfo.HdVersion = "CT100_700_V3.2";

    memset(&shareInfoSaleUi,0x00,sizeof(stTabSaleUi));
    memset(&shareInfoLoginUi,0x00,sizeof(stTabLoginUi));
    memset(&shareInfoPayUi,0x00,sizeof(stTabPayUi));
    memset(&shareInfoMenuUi,0x00,sizeof(stTabMenuUi));
    memset(&shareInfoCalibrationUi,0x00,sizeof(stTabCalibrationUi));
    memset(&m_stMessageInfo,0x00,sizeof(stMessageQT));
    memset(&m_stInputInfo,0x00,sizeof(stInputQT));
    memset(&m_stAddSaleInfo,0x00,sizeof(stAddSaleQT));
    memset(&shareReportUi,0x00,sizeof(stReportQT));
    memset(&m_stGoodsKeyInfo,0x00,sizeof(stGoodsKeyQT));

    sprintf(shareInfoSaleUi.ver,"%s",SOFTWARE_VER);
    sprintf(shareInfoSaleUi.systemName,"%s","浙江东日农贸管理系统");

    ATdb_Excute_SQL_ClearDisk();
    stp = new struct_tb_product[1];
    sta = new struct_tb_attach[1];
    stb = new struct_tb_business[1];

    QSqlQuery query;
    int dbSize;
    if(21 == (dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from TABLE_SCALESETTING ;",&query)))
    {
        bool ok;
        for(int i=0; i<dbSize;i++)
        {
            query.next();
            if(i == 0)
                m_ShareNetInfo.RemoteServer = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 1)
                m_ShareNetInfo.InternalServer = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i== 2)
                m_ShareNetInfo.Mac = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 3)
                m_ShareNetInfo.Ip = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 4)
                m_ShareNetInfo.Mask = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 5)
                m_ShareNetInfo.Gateway = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 6)
                m_ShareNetInfo.Net_Mode = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 7)
                m_ShareNetInfo.SSID = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 8)   
             m_ShareNetInfo.PassWord = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 9)
                m_ShareNetInfo.Encryption = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 10)
                m_ShareNetInfo.Dhcp = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 11)
                m_ShareNetInfo.Dns = query.record().value(TABLE_SCALESETTING_VALUE).toString();
            else if(i == 12)
                m_ShareSystemInfo.ScreenTimeout = query.record().value(TABLE_SCALESETTING_VALUE).toString().toInt(&ok,10);
            else if(i == 13)
            {
                m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT = query.record().value(TABLE_SCALESETTING_VALUE).toString().toInt(&ok,10);
                m_ShareSystemInfo.m_curDecimal.dec_pt_TARE = m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT;
                m_ShareSystemInfo.nWeightCoe = pow(10, m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT);
            }
            else if(i == 14)
            {
                m_ShareSystemInfo.m_curDecimal.round = query.record().value(TABLE_SCALESETTING_VALUE).toString().toInt(&ok,10);
                m_ShareSystemInfo.Round = m_ShareSystemInfo.m_curDecimal.round;
                m_ShareSystemInfo.m_curDecimal.dec_pt_TOTAL = 2;
                m_ShareSystemInfo.m_curDecimal.dec_pt_UNIT = m_ShareSystemInfo.m_curDecimal.dec_pt_TOTAL;
                m_ShareSystemInfo.nPricesCoe = pow(10, m_ShareSystemInfo.m_curDecimal.dec_pt_UNIT);
                m_ShareSystemInfo.Digit = pow(10, (m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT+m_ShareSystemInfo.m_curDecimal.dec_pt_UNIT-m_ShareSystemInfo.m_curDecimal.dec_pt_TOTAL));
            }
            else if(i == 15)
            {
                m_ShareSystemInfo.BackLight = query.record().value(TABLE_SCALESETTING_VALUE).toString().toInt(&ok,10);
            }
            else if(i == 16)
            {
                QString userSwitch = query.record().value(TABLE_SCALESETTING_VALUE).toString();
                char Switch[16] = {0};
                sprintf(Switch,"%s",userSwitch.toStdString().c_str());
                if(userSwitch.length()>=7)
                {
                    m_ShareUserSwitchInfo.TotalPriceSwitch = Switch[0] - '0';
                    m_ShareUserSwitchInfo.SavePluSwitch = Switch[1] - '0';
                    m_ShareUserSwitchInfo.SavePriceSwitch = Switch[2] - '0';
                    m_ShareUserSwitchInfo.RecordSwitch =  Switch[3] - '0';
                    m_ShareUserSwitchInfo.PrintSwitch = Switch[4] - '0';
                    m_ShareUserSwitchInfo.fThresholdVale = ((Switch[5]-'0')*10 + (Switch[6]-'0'))/100.00;
                }
            }
            else if(i == 17)
            {
                QString sysSwitch = query.record().value(TABLE_SCALESETTING_VALUE).toString();
                char Switch[20] = {0};
                sprintf(Switch,"%s",sysSwitch.toStdString().c_str());
                if(sysSwitch.length()>=15)
                {
                    m_ShareSysSwitchInfo.LoginMode = Switch[0] - '0';
                    if(m_ShareSysSwitchInfo.LoginMode == 0)
                    {
                        m_enQtUiType = enQt_SaleQT;
                        enLastShowType = enQt_LoginQT;
                    }
                    else
                    {
                        m_enQtUiType = enQt_LoginQT;
                        enLastShowType = enQt_LoginQT;
                    }
                    m_ShareSysSwitchInfo.PrintPaper = Switch[1] - '0';
                    m_ShareSysSwitchInfo.CashSwitch = Switch[2] - '0';
                    m_ShareSysSwitchInfo.CameraSwitch =  Switch[3] - '0';
                    m_ShareSysSwitchInfo.RfidSwitch = Switch[4] - '0';
                    m_ShareSysSwitchInfo.BluetoothSwitch = Switch[5]-'0';
                    m_ShareSysSwitchInfo.ZigbeeSwitch = Switch[6] - '0';
                    m_ShareSysSwitchInfo.PosSwitch = Switch[7] - '0';
                    m_ShareSysSwitchInfo.ScannerSwitch = Switch[8] - '0';
                    m_ShareSysSwitchInfo.ManageSwitch =  Switch[9] - '0';
                    m_ShareSysSwitchInfo.WebSwitch = Switch[10] - '0';
                    m_ShareSysSwitchInfo.AlipaySwitch = Switch[11]-'0';
                    m_ShareSysSwitchInfo.WechatSwitch = Switch[12] - '0';
                    m_ShareSysSwitchInfo.OtherSwitch = Switch[13] - '0';
                    m_ShareSysSwitchInfo.DebugSwitch = Switch[14] - '0';
                }
            }
        }
    }
    //printf("fThresholdVale:%f\n",m_ShareUserSwitchInfo.fThresholdVale);
    //printf("PrintPaper:%d\n",m_ShareSysSwitchInfo.PrintPaper);
    query.clear();
    if(25 == (dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from TABLE_USERSETTING ;",&query)))
    {
        for(int i=0; i<dbSize;i++)
        {
            query.next();
            if(i == 0)
                m_ShareCustomerInfo.AlipayId = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 1)
                m_ShareCustomerInfo.AlipayNo = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i== 2)
                m_ShareCustomerInfo.WechatId = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 3)
                m_ShareCustomerInfo.WechatNo = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 4)
                m_ShareCustomerInfo.WechatKey = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 5)
                m_ShareCustomerInfo.OtherID = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 6)
                m_ShareCustomerInfo.OtherNo = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 7)
                m_ShareCustomerInfo.BluetoothName = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 8)
                m_ShareCustomerInfo.BluetoothPsd = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 9)
                m_ShareCustomerInfo.ZigbeeNetID = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 10)
                m_ShareCustomerInfo.ZigbeeChannel = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 11)
                m_ShareDeviceInfo.DeviceId = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 12)
                m_ShareDeviceInfo.DeviceType = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 13)
                m_ShareDeviceInfo.BoothId = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 14)
            {
                m_ShareDeviceInfo.BoothName = query.record().value(TABLE_USERSETTING_VALUE).toString();
                if(m_ShareDeviceInfo.BoothName.length()>0)
                    sprintf(shareInfoSaleUi.sellerName,"商户:%s",m_ShareDeviceInfo.BoothName.toStdString().c_str());
            }
            else if(i == 15)
            {
                m_ShareDeviceInfo.BoothCode = query.record().value(TABLE_USERSETTING_VALUE).toString();
            }
            else if(i == 16)
                m_ShareDeviceInfo.ScaleNum = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 17)
                m_ShareDeviceInfo.MarketCode = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 18)
            {
                m_ShareDeviceInfo.MarketName = query.record().value(TABLE_USERSETTING_VALUE).toString();
                sprintf(shareInfoSaleUi.systemName,"%s",m_ShareDeviceInfo.MarketName.toStdString().c_str());
            }
            else if(i == 19)
                m_ShareDeviceInfo.HeadTail1 = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 20)
                m_ShareDeviceInfo.HeadTail2 = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 21)
                m_ShareDeviceInfo.HeadTail3 = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 22)
                m_ShareDeviceInfo.HeadTail4 = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 23)
                m_ShareDeviceInfo.HeadTail5 = query.record().value(TABLE_USERSETTING_VALUE).toString();
            else if(i == 24)
                m_ShareDeviceInfo.HeadTail6 = query.record().value(TABLE_USERSETTING_VALUE).toString();
        }
    }
    bool ok;
    if(m_ShareDeviceInfo.BoothCode.length()<=0 || m_ShareDeviceInfo.BoothCode.toInt(&ok,10) >100)
    {
        m_ShareDeviceInfo.BoothCode = "100";
    }
    query.clear();
    ///qDebug() << "deviceid"<<m_ShareDeviceInfo.DeviceId;
    QString Qstr;
    //查找最后一笔交易的businessCode
    if(ATdb_Select_ReturnQString_By_SQL_Statements("select businessCode from table_business order by id desc limit 1;",&Qstr)<0)
    {
        qDebug() << "CShareInfo select businessCode error";
    }
    if(Qstr.length()>0)
    {

        m_ShareSaleInfo.BusinessCode = Qstr.toInt(&ok,10);
    }
    else{
        m_ShareSaleInfo.BusinessCode = 0;
    }

    Qstr.clear();
    if(ATdb_Select_ReturnQString_By_SQL_Statements("select fullarrange from table_calibration where id=1;",&Qstr) > 0)
    {
        int nFullarrange = Qstr.toInt(&ok,10);
        Qstr.clear();
        if(ATdb_Select_ReturnQString_By_SQL_Statements("select e1 from table_calibration where id=1;",&Qstr) > 0)
        {
            int nE = Qstr.toInt(&ok,10);
            m_ShareSystemInfo.MaxWeight = (nFullarrange*1000+9*nE) ;
        }
        Qstr.clear();
        if(ATdb_Select_ReturnQString_By_SQL_Statements("select e from table_calibration where id=1;",&Qstr) > 0)
        {
            int nE = Qstr.toInt(&ok,10);;
            m_ShareSystemInfo.MinWeight = 20*nE;
        }
    }
}

CShareInfo::~CShareInfo(void)
{

}

int CShareInfo::init()
{
    StartNetwork();
    //初初始化系统时间和网络
    SystemTimeInit();
    //共享内存通讯
    m_pShm = new shmCom(&m_ShareSystemInfo.m_curDecimal);
    if(m_pShm)
    {
        m_pShm->initShm();
        m_pShm->SetMaxWeight(m_ShareSystemInfo.MaxWeight);
    }
    //初始化与单片机通讯
    m_pAtmel8 = new CAtmSerial();
    if(m_pAtmel8)
    {
        m_pAtmel8->Init();
    }
    //初始化打印机设备
    m_pPrint = new CReceiptThermal();
    if(m_pPrint)
    {
        m_pPrint->init();
    }
    //初始化按键设备
#if (KEY_TYPE == MIDDLE_KEY)
    {
        ATKey_init(KEY_TYPE);
    }
#else
    {
        ATKey_init(KEY_TYPE);
    }
#endif
    DeteleBatchInfo();
    DeteleSaleInfo();

    pthread_t idAD;
    int pthread = pthread_create(&idAD, NULL, theadAD, this);
    if (pthread)
    {
        errno = pthread;
        perror("create theadAD thread");
    }
    pthread = pthread_detach(idAD);
    if (pthread) {
        errno = pthread;
        perror("detach theadAD thread");
    }

    pthread_t idCheckNet;
    pthread = pthread_create(&idCheckNet, NULL, theadSystemCheckout, this);
    if (pthread)
    {
        errno = pthread;
        perror("create theadSystemCheckout thread");
    }
    pthread = pthread_detach(idCheckNet);
    if (pthread) {
        errno = pthread;
        perror("detach theadSystemCheckout thread");
    }

    pthread_t idBackLight;
    pthread = pthread_create(&idBackLight, NULL, theadOverBackLight, this);
    if (pthread)
    {
        errno = pthread;
        perror("create theadOverBackLight thread");
    }
    pthread = pthread_detach(idBackLight);
    if (pthread) {
        errno = pthread;
        perror("detach theadOverBackLight thread");
    }
}

int CShareInfo::UpdateGui(void)
{
    if (!m_pShm->m_bReady)
    {
        return -1;
    }
    switch(m_enQtUiType)
    {
        case enQt_SaleQT:
            m_pShm->show((char*)&shareInfoSaleUi,m_enQtUiType);
            /*
            if(strlen(shareInfoSaleUi.downloadUrl)>0)
                memset(shareInfoSaleUi.downloadUrl,0x00,LEN64);             //确保QT不会重复下载URL
            if(strlen(shareInfoSaleUi.picturlUrl)>0)                         //确保QT不会重复下载URL
                memset(shareInfoSaleUi.picturlUrl,0x00,LEN64);
            */

            break;
        case enQt_LoginQT:
            m_pShm->show((char*)&shareInfoLoginUi,m_enQtUiType);
            break;       
        case enQt_PayQT:
            m_pShm->show((char*)&shareInfoPayUi,m_enQtUiType);
            break;
        case enQt_MenuQT:
            m_pShm->show((char*)&shareInfoMenuUi,m_enQtUiType);
            break;
        case enQt_CalQt:
            m_pShm->show((char*)&shareInfoCalibrationUi,m_enQtUiType);
            break;
        case enQt_ReportQt:
            m_pShm->show((char*)&shareReportUi,m_enQtUiType);
            break;
        case enQt_GoodsKeyQT:
            m_pShm->show((char*)&m_stGoodsKeyInfo,m_enQtUiType);
            break;
        case enQt_MsgQT:
            m_pShm->show((char*)&m_stMessageInfo,m_enQtUiType);
            break;
        case enQt_InputQT:
            m_pShm->show((char*)&m_stInputInfo,m_enQtUiType);
            break;
        case enQt_AddSaleQT:
             m_pShm->show((char*)&m_stAddSaleInfo,m_enQtUiType);
            break;
        default:
            break;
    }
}

int CShareInfo:: RefreshWeight(void)
{
    if(GetSaleMode() == enPieces)
    {
        return 0;
    }
    adc_ret curAd = m_LastAdInfo;

    //m_adInfo.kg = 1000;
    if ((m_adInfo.kg != curAd.kg) || (m_adInfo.kgtara != curAd.kgtara) || (m_adInfo.stabok != curAd.stabok)
            || (m_adInfo.zerook != curAd.zerook) || (m_adInfo.taraok != curAd.taraok))
    {
            m_LastAdInfo = m_adInfo;
            m_bWeightChange = 1;
            memset(shareInfoSaleUi.tare, 0, LEN10);
            memset(shareInfoSaleUi.weight, 0, LEN10);
            //printf("RefreshWeight:%ld\n",m_adInfo.kg);
            if(m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 3)
            {
                sprintf(shareInfoSaleUi.weight, "%ld", m_adInfo.kg/1);
                //printf("RefreshWeight:%s\n",shareInfoSaleUi.weight);
                sprintf(shareInfoSaleUi.tare, "%ld", m_adInfo.kgtara/1);
            }
            else if(m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT == 2)
            {
                sprintf(shareInfoSaleUi.weight, "%ld", m_adInfo.kg/10);
                //printf("RefreshWeight:%s\n",shareInfoSaleUi.weight);
                sprintf(shareInfoSaleUi.tare, "%ld", m_adInfo.kgtara/10);
            }

            if (m_adInfo.stabok == 0)
            {
                shareInfoSaleUi.stable = false;
            }
            else
            {
                shareInfoSaleUi.stable = true;
            }

            if (m_adInfo.zerook == 0)
            {
                shareInfoSaleUi.zero = false;
            }
            else
            {
                shareInfoSaleUi.zero = true;
            }

            // 只要皮重不等于0，就显示net
            if (m_adInfo.taraok == 0)
            {
                    shareInfoSaleUi.net = false;
            }
            else
            {
                    shareInfoSaleUi.net = true;
            }
            return 1;
    }
    return 0;
}
char CShareInfo::KeyValueConversion(unsigned char key)
{
    static bool m_ShifFlag = false;
    if (key >= _KEY_PLU1 &&  key <= _KEY_PLU26)
    {
        unsigned ch = key - _KEY_PLU1;
        if(m_ShifFlag)
            return ch+'A';
        else
            return ch+'a';
    }
    else if(key >= _KEY_NUM0 &&  key <= _KEY_NUM9)
    {
        unsigned ch = key - _KEY_NUM0;
        return ch+'0';
    }
    else if(key == _KEY_SHIFT)
    {
        if(m_ShifFlag == true)
        {
            m_ShifFlag = false;
        }
        else
        {
            m_ShifFlag = true;
        }
        return 0;
    }
    else
    {
        switch(key)
        {
            case _KEY_DOT:
                return '.';
#if (KEY_TYPE == SMALL_KEY)
        {
            case _KEY_PLU27:
                if(m_ShifFlag)
                    return ' ';
                else
                    return ',';
            case _KEY_PLU28:
                if(m_ShifFlag)
                    return '-';
                else
                    return '_';
            case _KEY_PLU29:
                if(m_ShifFlag)
                    return '/';
                else
                    return '\\';
            case _KEY_PLU30:
                if(m_ShifFlag)
                    return ':';
                else
                    return ';';
        }
#elif (KEY_TYPE == MIDDLE_KEY)
        {
            case _KEY_PLU27:
                return '~';
            case _KEY_PLU28:
                return '!';
            case _KEY_PLU29:
                return '@';
            case _KEY_PLU30:
                return '#';
            case _KEY_PLU31:
                return '$';
            case _KEY_PLU32:
                return '%';
            case _KEY_PLU33:
                return '^';
            case _KEY_PLU34:
                return '&';
            case _KEY_PLU35:
                return '*';
            case _KEY_PLU36:
                return '(';
            case _KEY_PLU37:
                return ';';
            case _KEY_PLU38:
                return '-';
            case _KEY_PLU39:
                return '+';
            case _KEY_PLU40:
                return '_';
            case _KEY_PLU41:
                return '=';
            case _KEY_PLU42:
                return '{';
            case _KEY_PLU43:
                return '}';
            case _KEY_PLU44:
                return '[';
            case _KEY_PLU45:
                return ']';
            case _KEY_PLU46:
                return '?';
            case _KEY_PLU47:
                return '"';
            case _KEY_PLU48:
                return ')';
            case _KEY_PLU49:
                return ':';
            case _KEY_PLU50:
                return '/';
            case _KEY_PLU51:
                return '\\';
            case _KEY_PLU52:
                return '<';
            case _KEY_PLU53:
                return '>';
            case _KEY_PLU54:
                return '?';
            case _KEY_PLU55:
                return '|';
            case _KEY_PLU56:
                return ',';
            case _KEY_PLU57:
                return ' ';
         }
#endif
        }
    }
    return 0;
}
int CShareInfo::PerformSqlFile(char *filepath)
{
   QFile file(filepath);
   if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       qDebug()<<"Can't open the file!"<<endl;
       return 0;
   }
   while(!file.atEnd())
   {
       QByteArray line = file.readLine();
       QString str(line);
       //qDebug()<< str;
       if(ATdb_Excute_SQL_Statements(str)<0)
       {
           qDebug()<<"exe sql error "<<endl;
           break;
       }
   }
   file.seek(0);
   file.close();
}
void CShareInfo::SystemTimeInit(void)
{
    char strTime[24] = {0};
    char Time[30] = {0};
    ATRtcRead(strTime);
    printf("________strTime: %s\n", strTime);
    // 写时间到系统时钟
    // date -s "2015-03-02 11:16:02"
    sprintf(Time,"date -s \"%s\"", strTime);
    system(Time);
}

int CShareInfo::StartNetwork(void)
{
    system("ifconfig wlan0 down");
    system("ifconfig eth0 down");

    char commond[256];
    // 0:lan ,1:wlan
    if (m_ShareNetInfo.Net_Mode.compare("1") == 0)
    {
      //  system("pkill -9 wpa_supplicant");
        //连接影藏的AP
//        memset(commond, 0x00, 256);
//        sprintf(commond, "%s", "/wifi_8188/command/wpa_cli -p/wifi_8188/configure/wpa_supplicant set_network 0 scan_ssid 1");
//        system(commond);
//        printf("%s\n", commond);

        memset(commond, 0x00, 256);
        char * s = "/wifi_8188/command/wpa_cli -p/wifi_8188/configure/wpa_supplicant set_network 0 ssid '\"%s\"'";
        sprintf(commond, s, m_ShareNetInfo.SSID.toStdString().c_str());
        system(commond);
        printf("%s\n", commond);

        if(m_ShareNetInfo.PassWord.length() > 0)
        {
            char * s1 = "/wifi_8188/command/wpa_cli -p/wifi_8188/configure/wpa_supplicant set_network 0 psk '\"%s\"'";
            memset(commond, 0x00, 256);
            sprintf(commond, s1, m_ShareNetInfo.PassWord.toStdString().c_str());
            system(commond);
            printf("%s\n", commond);
        }
        else//路由器无密码
        {
            system("pkill -9 hostapd");
            system("/wifi_8188/command/wpa_cli -p/wifi_8188/configure/wpa_supplicant set_network 0 key_mgmt NONE");
        }
        system("/wifi_8188/command/wpa_cli -p/wifi_8188/configure/wpa_supplicant select_network 0");
        system("ifconfig wlan0 up");

        if(m_ShareNetInfo.Dhcp.compare("1") == 0)
        {
            FILE *fp = popen("udhcpc -n -i wlan0","r");
            if(!fp)
            {
                printf("udhcpc -n -i wlan0 fail !\n");
            }
            char s[64] = {0x00};
            while((fgets(s,64,fp))!=NULL)
             {
                  char *start = NULL;
                  const char *ipPrefix = "Sending select for ";
                  const char *dnsPrefix = "adding dns ";
                  start = strstr(s,ipPrefix);
                  if(start)
                  {
                    start = start+strlen(ipPrefix);
                    char *end = start;
                    while(*(++end) != '\n')
                    {
                        if(*end == '.' && *(end+1) == '.')
                        {
                            *end == '\0';
                            break;
                        }
                    }
                    *end = '\0';
                    printf("ip----%s\n",start);
                    m_ShareNetInfo.Ip = start;
                    ATdb_Excute_SQL_Statements(QString("update table_scalesetting set value='%1' where id=4;").arg(m_ShareNetInfo.Ip));
                  }
                  start = NULL;
                  start = strstr(s,dnsPrefix);
                  if(start)
                  {
                    start = start+strlen(dnsPrefix);
                    for(int i=1;i<16;i++)
                    {
                        if(*(start+i) == '\n' || *(start+i) == '\r')
                        {
                            *(start+i) = '\0';
                            break;
                        }
                    }
                    printf("dns----%s\n",start);
                    m_ShareNetInfo.Dns = start;
                    m_ShareNetInfo.Gateway = start;
                    ATdb_Excute_SQL_Statements(QString("update table_scalesetting set value='%1' where id=6;").arg(m_ShareNetInfo.Dns));
                    ATdb_Excute_SQL_Statements(QString("update table_scalesetting set value='%1' where id=12;").arg(m_ShareNetInfo.Dns));


                  }
             }
            pclose(fp);
            system("ifconfig wlan0 up");
        }
        else
        {
//            sprintf(commond, "ifconfig wlan0 hw ether %s", m_ShareNetInfo.Mac.toStdString().c_str());
//            system(commond);
//            printf("%s\n", commond);

            char * s2 = "ifconfig wlan0 %s netmask %s";
            memset(commond, 0x00, 256);
            sprintf(commond, s2, m_ShareNetInfo.Ip.toStdString().c_str(), m_ShareNetInfo.Mask.toStdString().c_str());
            system(commond);

            char * s3 = "route add default gw %s";
            memset(commond, 0x00, 256);
            sprintf(commond, s3, m_ShareNetInfo.Gateway.toStdString().c_str());
            system(commond);

            system("ifconfig wlan0 up");
        }
    }
    else
    {
        //shareInfoSaleUi.wifi  = enLANT;
        system("ifconfig wlan0 down");

//        sprintf(commond, "ifconfig eth0 hw ether %s", m_ShareNetInfo.Mac.toStdString().c_str());
//        system(commond);
//        printf("%s\n", commond);

        char * s2 = "ifconfig eth0 %s netmask %s";
        memset(commond, 0x00, 120);
        sprintf(commond, s2, m_ShareNetInfo.Ip.toStdString().c_str(), m_ShareNetInfo.Mask.toStdString().c_str());
        system(commond);
        printf("%s\n", commond);

        char * s3 = "route add default gw %s";
        memset(commond, 0x00, 120);
        sprintf(commond, s3, m_ShareNetInfo.Gateway.toStdString().c_str());
        system(commond);
        printf("%s\n", commond);

        system("ifconfig eth0 up");
        usleep(50 *1000);
        if(m_ShareNetInfo.Dhcp.compare("1") == 0)
        {
            usleep(50 *1000);
            FILE *fp = popen("udhcpc -n -i eth0","r");
            if(!fp)
            {
                printf("udhcpc -n -i eth0 fail !\n");
            }
            char s[64] = {0x00};
            while((fgets(s,64,fp))!=NULL)
             {
                  char *start = NULL;
                  const char *ipPrefix = "Sending select for ";
                  const char *dnsPrefix = "adding dns ";
                  start = strstr(s,ipPrefix);
                  if(start)
                  {
                    start = start+strlen(ipPrefix);
                    char *end = start;
                    while(*(++end) != '\n')
                    {
                        if(*end == '.' && *(end+1) == '.')
                        {
                            *end == '\0';
                            break;
                        }
                    }
                    *end = '\0';
                    printf("ip----%s\n",start);
                    m_ShareNetInfo.Ip = start;
                    ATdb_Excute_SQL_Statements(QString("update table_scalesetting set value='%1' where id=4;").arg(m_ShareNetInfo.Ip));
                  }
                  start = NULL;
                  start = strstr(s,dnsPrefix);
                  if(start)
                  {
                    start = start+strlen(dnsPrefix);
                    for(int i=1;i<16;i++)
                    {
                        if(*(start+i) == '\n' || *(start+i) == '\r')
                        {
                            *(start+i) = '\0';
                            break;
                        }
                    }
                    printf("dns----%s\n",start);
                    m_ShareNetInfo.Dns = start;
                    ATdb_Excute_SQL_Statements(QString("update table_scalesetting set value='%1' where id=6;").arg(m_ShareNetInfo.Dns));
                    ATdb_Excute_SQL_Statements(QString("update table_scalesetting set value='%1' where id=12;").arg(m_ShareNetInfo.Dns));
                  }
             }
            pclose(fp);
        }
    }
}
int CShareInfo::ZeroTare(unsigned char key)
{
    static long ZeroVaue = 0;
    bool ret = false;
    adc_param param = ATAd_getParam();
    switch(key)
    {
        case _KEY_ZERO:
            if( param.full_arrange *1000/50 >= abs(atoi(shareInfoSaleUi.weight)))
            {
                //在连续置零的情况下，不能超过最大置零范围
                if(ZeroVaue + atoi(shareInfoSaleUi.weight) > param.full_arrange *1000/50)
                {
                    ATBeepErr();
                    this->ShowMessage("报警:[累计置零]重量超出置零范围");
                    break;
                }
                ret = ATAd_setZero();
                if (ret == true)
                {
                    ZeroVaue = ZeroVaue + atoi(shareInfoSaleUi.weight);
                    ATBeep();
                }
            }
            else
            {
                this->ShowMessage("报警:重量超出置零范围");
                ATBeepErr();
            }
             break;

        case _KEY_TARE:
            if( (param.full_arrange *1000/2 - param.e) >= abs(atoi(shareInfoSaleUi.weight)))
            {
                //在连续去皮的情况下，不能超过最大去皮范围
               if(atoi(shareInfoSaleUi.tare) + atoi(shareInfoSaleUi.weight) > (param.full_arrange *1000/2 - param.e))
               {
                   ATBeepErr();
                   this->ShowMessage("报警:[累计去皮]皮重大于去皮范围");
                   break;
               }
                ret = ATAd_setTara();
                if (ret == true)
                {
                    adc_ret ret;
                    ATAd_getKg(&ret);
                    unsigned int weight = m_ShareSystemInfo.MaxWeight - ret.kgtara;
                    m_pShm->SetMaxWeight(weight);
                    ATBeep();
                }
            }
            else
            {
                this->ShowMessage("报警:重量超出去皮范围");
                ATBeepErr();
            }
            break;
         defualt:break;
    }
    return 0;
}
int CShareInfo::ShowScreenBackLight(void)
{
    static unsigned int nBackLightTime = 0;
    static bool bBackLight = false;
    unsigned char ch;
    if(m_ShareSystemInfo.ScreenTimeout == 0)
    {
        return 0;
    }
    if(bBackLight)
    {
        if ((SALE_KEY_VALID == ATKey_getKey(&ch)) || (m_LastAdInfo.kg != m_adInfo.kg))
        {
            nBackLightTime = 0;
            bBackLight = false;
            ATPwm_SetBackLight(8);
        }
    }
    else
    {
        if(m_LastAdInfo.kg != m_adInfo.kg || ATKey_getStates()==true)
        {
            nBackLightTime = 0;
        }

        if ((++nBackLightTime >= m_ShareSystemInfo.ScreenTimeout *20))
        {
            nBackLightTime = 0;
            printf("------------------------backlight 0\n");
            bBackLight = true;
            ATPwm_SetBackLight(0);
        }
    }
}
//用于超重报警和开盖报警
int CShareInfo::ShowOverWeightWarningProc(enMsgType MsgType, char *pTitle, char *pMessage)
{
    printf("---------------------------ShowOverWeightWarningProc\n");
    enQtShowType lastType = m_enQtUiType;
    m_enQtUiType = enQt_MsgQT;
    m_stMessageInfo.cmd = MSG_MESSAGE_REQ;	//请求还是确认消息

    sprintf(m_stMessageInfo.title,"%s",pTitle);
    sprintf(m_stMessageInfo.message,"%s",pMessage);
    m_pShm->show((char*)&m_stMessageInfo,m_enQtUiType);

    ATBeepErr();
    unsigned char ch = 0;
    while(1)
    {
        usleep(500*1000);
        if(SALE_KEY_VALID == ATKey_getKey(&ch))
        {
            m_stMessageInfo.cmd = MSG_MESSAGE_OK;
            m_pShm->show((char*)&m_stMessageInfo,m_enQtUiType);
            break;
        }
        if (m_adInfo.kg <= m_pShm->m_nMaxWeight)
        {
            m_stMessageInfo.cmd = MSG_MESSAGE_OK;
            m_pShm->show((char*)&m_stMessageInfo,m_enQtUiType);
            break;
        }
    }
   m_enQtUiType = lastType;
   m_pShm->show((char*)&m_stMessageInfo,m_enQtUiType);
   printf("---------------------------return ShowOverWeightWarningProc\n");
}
int CShareInfo::ShowMessage(char *pMessage)
{
    if(strlen(pMessage)>0)
    {
        memset (shareInfoSaleUi.message, 0, sizeof(shareInfoSaleUi.message));
        sprintf(shareInfoSaleUi.message,"%s",pMessage);
    }
    else
    {
        memset (shareInfoSaleUi.message, 0, sizeof(shareInfoSaleUi.message));
    }
    UpdateGui();
}

int CShareInfo::ReadS70CardInfo(void)
{
    return ATSerialDevice_Rfid_Find();
}
int CShareInfo::ReadCPUCardInfo(void)
{
    return ATSerialDevice_Rfid_Find();
}
int CShareInfo::Rfid_Find(void)
{
    return ATSerialDevice_Rfid_Find();
}

int CShareInfo::DackTime(char *startTime , char *backTime , int backTimesDay ,bool flag)
{
    time_t timep;
   //startTime is NULL or 2018-10-10 10:10:10 . other error
   if(startTime == NULL || strlen(startTime) <= 0)
   {
       time(&timep);
   }
   else
   {
        struct tm timerp;
        strptime(startTime,"%Y-%m-%d %H:%M:%S",&timerp);
        timep = mktime(&timerp);
   }

   timep -= 60 *60 *24 * backTimesDay;
   struct tm *p;
   p = localtime(&timep);

   if(flag == false)
   {
       sprintf(backTime,"%d%02d%02d",(1900 + p->tm_year),( 1 + p->tm_mon), p->tm_mday);
   }
   else
   {
       sprintf(backTime,"%d-%02d-%02d",(1900 + p->tm_year),( 1 + p->tm_mon), p->tm_mday);
   }

   printf("backTime:%s\n",backTime);
}
int CShareInfo::DeteleBatchInfo()
{
    int backTimesDay = 5;
    char startTime[32] = {0};
    char endTime[32] = {0};
    ATRtcRead(startTime);
    printf("currentTime:%s\n",startTime);

    DackTime(startTime,endTime,backTimesDay,true);//找到time时间以前的时间点
    startTime[10] = '\0';
    QString sqlStr="";
    sqlStr.sprintf("delete from TABLE_BATCH where TIME<'%s' or TIME>'%s';",endTime,startTime);
    ATdb_Excute_SQL_Statements(sqlStr);
}

int CShareInfo::DeteleSaleInfo()
{
    int backTimesDay = 7;
    char startTime[32] = {0};
    char endTime[32] = {0};
    ATRtcRead(startTime);
    //printf("currentTime:%s\n",startTime);

    DackTime(startTime,endTime,backTimesDay,true);//找到time时间以前的时间点
    //strcat(endTime," 23:59:59");
    startTime[10] = '\0';
    QString sqlStr="";
    //sqlStr.sprintf("delete from TABLE_ATTACH where WebUp='1' and SCALETIME<'%s 00:00:00' or SCALETIME>'%s 23:59:59';",startTime,startTime);
    //ATdb_Excute_SQL_Statements(sqlStr);
    //sqlStr.clear();
    sqlStr.sprintf("delete from TABLE_ATTACH where  SCALETIME<'%s 00:00:00' or SCALETIME>'%s 23:59:59';",endTime,startTime);
    ATdb_Excute_SQL_Statements(sqlStr);

    //sqlStr.clear();
    //sqlStr.sprintf("delete from TABLE_BUSINESS where webUp='1' and time<'%s 00:00:00' or time>'%s 23:59:59';",startTime,startTime);
    //ATdb_Excute_SQL_Statements(sqlStr);
    sqlStr.clear();
    sqlStr.sprintf("delete from TABLE_BUSINESS where  time<'%s 00:00:00' or time>'%s 23:59:59';",endTime,startTime);
    ATdb_Excute_SQL_Statements(sqlStr);
    //ATdb_Excute_SQL_ClearDisk();

//    DackTime(startTime,endTime,1,false);//找到time时间以前的时间点

//    //删除 当天和前一天 之外的所有图片
//    char del[64] = {0};
//    system("cd /root/Application/photo/");
//    sprintf(del,"find /root/Application/photo/* |grep -v %s* |xargs rm -f" ,endTime);
//    printf("%s\n",del);
//    system(del);
//    system("cd -");
}
