#include "salefunction.h"

CSaleFunction::CSaleFunction(CShareInfo *pInfo)
{
    m_pSaleFunShareInfo = pInfo;
}
CSaleFunction::~CSaleFunction(void)
{

}
int CSaleFunction::ExcutCustomSqls(void)
{
    int nRet = 0;
    nRet = system("mount /dev/sda1 /mnt");
    if (nRet != 0)
    {
        nRet = system("mount /dev/sdb1 /mnt");
        if (nRet != 0)
        {
            nRet = system("mount /dev/sdc1 /mnt");
            if (nRet != 0)
            {
                printf("3-------------------------nRet:%d\n", nRet);
                return -1;
            }
        }
    }
    m_pSaleFunShareInfo->PerformSqlFile("/mnt/release/sql.txt");
    system("umount /mnt");
    return 0;
}

int CSaleFunction::ExportFile(void)
{
    int nRet = 0;
    nRet = system("mount /dev/sda1 /mnt");
    if (nRet != 0)
    {
        nRet = system("mount /dev/sdb1 /mnt");
        if (nRet != 0)
        {
            nRet = system("mount /dev/sdc1 /mnt");
            if (nRet != 0)
            {
                    printf("3-------------------------nRet:%d\n", nRet);
                    return -1;
            }
        }
    }
    nRet = system("cp -rf  /root/Application/photo/* /mnt/release/");
    nRet = system("cp -rf  /root/Application/database/* /mnt/release/");
    nRet = system("umount /mnt");
    return 0;
}
int CSaleFunction::ExcutScript(void)
{
    int nRet = 0;
    nRet = system("mount /dev/sda1 /mnt");
    if (nRet != 0)
    {
            nRet = system("mount /dev/sdb1 /mnt");
            if (nRet != 0)
            {
                    nRet = system("mount /dev/sdc1 /mnt");
                    if (nRet != 0)
                    {
                            return -1;
                    }
            }
    }
     system("/mnt/release/copyFile.sh");
     system("umount /mnt");
     return 0;
}
int CSaleFunction::ExcutRestCmd(void)
{
    int nRet = 0;
    nRet = system("mount /dev/sda1 /mnt");
    if (nRet != 0)
    {
        nRet = system("mount /dev/sdb1 /mnt");
        if (nRet != 0)
        {
            nRet = system("mount /dev/sdc1 /mnt");
            if (nRet != 0)
            {
                    return -1;
            }
        }
    }
    system("/mnt/release/s-update.sh");
    system("umount /mnt");
    return 0;
}

int CSaleFunction::TestNetwork(void)
{
    int nPingRet = 0;
    static unsigned char nPingCnt = 0;
    char strSystem[32] = {0};
    sprintf(strSystem, "ping %s -c 1", m_pSaleFunShareInfo->m_ShareNetInfo.Gateway.toStdString().c_str());
    while(1)
    {
        usleep(500*1000);
        nPingRet = system(strSystem);
        printf("nPingRet:%d\n", nPingRet);	//-1:fail
        if (nPingRet != 0)
        {
            if (++nPingCnt > 2)
            {
                    nPingCnt = 0;
                    return -1;
            }
        }
        else
        {
            nPingCnt = 0;
            break;
        }
    }
    return 0;
}

int CSaleFunction::PrintLastSale(void)
{
     QSqlQuery staQuery;
    //查找最后一笔交易小计
    if(ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from table_attach order by id desc limit 1;",&staQuery) != 1)
    {
        qDebug() << "PrintLastSale select table_attach error";
    }
    staQuery.next();

    QString Qstr = QString("select * from table_business where businessCode=%1;").arg(staQuery.record().value(TABLE_ATTACH_BUSINESSCODE).toString());
    QSqlQuery stbQuery;
    int dbSize = 0;
    if( 1 > (dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(Qstr,&stbQuery)))
    {
        qDebug() << "PrintLastSale select table_business error or not data";
    }
    char str[64] = {0};

    stReceiptData data;
    data.type = enCenterString;
    ATLib_u2g((char *)m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail6.toStdString().c_str(),strlen(m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail6.toStdString().c_str()),str,64);
    //XX流通追溯体系
    sprintf(data.str,"%.32s",str);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    data.type = enString;
    sprintf(str,"市场名称：%.32s(补打)",m_pSaleFunShareInfo->m_ShareDeviceInfo.MarketName.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    data.type = enString;
    sprintf(data.str,"%s","--------------------------------");
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str,"摊位号: %s ", m_pSaleFunShareInfo->m_ShareDeviceInfo.BoothId.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str,"商  户: %s ", m_pSaleFunShareInfo->m_ShareDeviceInfo.BoothName.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str,"交易时间: %s ", staQuery.record().value(TABLE_ATTACH_SCALETIME).toString().toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str,"订单编号: %s ",staQuery.record().value(TABLE_ATTACH_SERIAL).toString().toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    // 每行最大长度为32
    sprintf(data.str,"%s","================================");
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "%s", "名称 重量/件数  单价/元 金额/元");
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(data.str,"%s","================================");
    m_pSaleFunShareInfo->m_pPrint->add(data);

    for (int i=0; i<dbSize; i++)
    {
        stbQuery.next();
        data.type = enString;
        if (stbQuery.record().value(TABLE_BUSINESS_SALEMODE).toString() == "1")
        {
            QString goodsName = stbQuery.record().value(TABLE_BUSINESS_GOODSNAME).toString();
            goodsName.append("(计件Pcs)");
            ATLib_u2g((char *)goodsName.toStdString().c_str(),strlen(goodsName.toStdString().c_str()),data.str,64);
            m_pSaleFunShareInfo->m_pPrint->add(data);
            sprintf(data.str, "%04s%08s %09s%08s", " ", stbQuery.record().value(TABLE_BUSINESS_PIECES).toString().toStdString().c_str(),
                    stbQuery.record().value(TABLE_BUSINESS_PRICE).toString().toStdString().c_str(),
                    stbQuery.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
            m_pSaleFunShareInfo->m_pPrint->add(data);
        }
        else
        {
            QString goodsName = stbQuery.record().value(TABLE_BUSINESS_GOODSNAME).toString();
            goodsName.append("(计重Kg)");
            ATLib_u2g((char *)goodsName.toStdString().c_str(),strlen(goodsName.toStdString().c_str()),data.str,64);
            m_pSaleFunShareInfo->m_pPrint->add(data);
            sprintf(data.str, "%04s%08s %09s%08s", " ", stbQuery.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString().c_str(),
                    stbQuery.record().value(TABLE_BUSINESS_PRICE).toString().toStdString().c_str(),
                    stbQuery.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString().c_str());
            m_pSaleFunShareInfo->m_pPrint->add(data);
        }
    }
    data.type = enLine;
    m_pSaleFunShareInfo->m_pPrint->add(data);

    data.type = enString;
    sprintf(str,"%s:  %d笔      %s元", "  合  计", dbSize, staQuery.record().value(TABLE_ATTACH_SUBTOTAL).toString().toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    if(m_pSaleFunShareInfo->m_ShareDeviceInfo.BoothCode != "100")
    {
        data.type = enString;
        sprintf(str,"%s:  商户或系统%s折优惠", "折扣优惠", m_pSaleFunShareInfo->m_ShareDeviceInfo.BoothCode.toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

        data.type = enString;
        sprintf(str,"%s:  %s元", "应收金额", staQuery.record().value(TABLE_ATTACH_PAYMONEY).toString().toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }

    bool ok;
    int PayMode = staQuery.record().value(TABLE_ATTACH_CARDTYPE).toString().toInt(&ok,10);
    if(PayMode == enBankPay)
    {
        sprintf(str,"%s:  [%s]", "支付方式", "扫码支付");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    else if(PayMode == enVipPay)
    {
        sprintf(str,"%s:  [%s]", "支付方式", "会员卡支付");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    else
    {
        sprintf(str,"%s:  [%s]", "支付方式", "现金支付");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    /*
    else if(PayMode == "2")
    {
        sprintf(str,"%s:  [%s]", "支付方式", "银联卡支付");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    else if(PayMode == "3")
    {
        sprintf(str,"%s:  [%s]", "支付方式", "支付宝支付");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    else if(PayMode == "4")
    {
        sprintf(str,"%s:  [%s]", "支付方式", "微信支付");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    else if(PayMode == "5")
    {
        sprintf(str,"%s:  [%s]", "支付方式", "会员卡支付");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    else if(PayMode == "6")
    {
        sprintf(str,"%s:  [%s]", "支付方式", "市民卡支付");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }*/

    data.type = enLine;
    m_pSaleFunShareInfo->m_pPrint->add(data);

    if(m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail5.length()>0)
    {
        data.type = enQR;
        sprintf(data.str,"%s%s",staQuery.record().value(TABLE_ATTACH_SERIAL).toString().toStdString().c_str(),
                m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail5.toStdString().c_str());
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    if(m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail1.length()>0)
    {
        data.type = enString;
        sprintf(data.str,"%s", m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail1.toStdString().c_str());
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    if(m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail2.length()>0)
    {
        data.type = enString;
        sprintf(data.str,"%s", m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail2.toStdString().c_str());
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }
    if(m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail3.length()>0)
    {
        data.type = enString;
        sprintf(data.str,"%s", m_pSaleFunShareInfo->m_ShareDeviceInfo.HeadTail3.toStdString().c_str());
        m_pSaleFunShareInfo->m_pPrint->add(data);
    }

    data.type = enStartPrint;
    m_pSaleFunShareInfo->m_pPrint->Print();
}
int CSaleFunction::PrintDeviceInfo(void)
{
    stReceiptData data;

    char str[64] = {0};

    data.type = enBigString;
    PrintBigCenter("电子秤信息", str);
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    // 打印设备信息
    data.type = enString;
    sprintf(str, "%s", "设备信息");
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "设备ID: %s", m_pSaleFunShareInfo->m_ShareDeviceInfo.DeviceId.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "设备类型: %s",  m_pSaleFunShareInfo->m_ShareDeviceInfo.DeviceType.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "软件版本号: %s", m_pSaleFunShareInfo->m_ShareSystemInfo.SwVersion.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "硬件版本号: %s", m_pSaleFunShareInfo->m_ShareSystemInfo.HdVersion.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    data.type = enLine;
    m_pSaleFunShareInfo->m_pPrint->add(data);

    data.type = enString;
    sprintf(str, "%s", "网络信息");
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "外网地址: %s", m_pSaleFunShareInfo->m_ShareNetInfo.RemoteServer.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "内网地址: %s", m_pSaleFunShareInfo->m_ShareNetInfo.InternalServer.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "MAC地址: %s", m_pSaleFunShareInfo->m_ShareNetInfo.Mac.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "IP地址: %s", m_pSaleFunShareInfo->m_ShareNetInfo.Ip.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "子网掩码: %s", m_pSaleFunShareInfo->m_ShareNetInfo.Mask.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "网关: %s", m_pSaleFunShareInfo->m_ShareNetInfo.Gateway.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "网络模式: %s", m_pSaleFunShareInfo->m_ShareNetInfo.Net_Mode.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "SSID: %s", m_pSaleFunShareInfo->m_ShareNetInfo.SSID.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "无线密码: %s", m_pSaleFunShareInfo->m_ShareNetInfo.PassWord.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "加密方式: %s", m_pSaleFunShareInfo->m_ShareNetInfo.Encryption.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "DHCP: %s", m_pSaleFunShareInfo->m_ShareNetInfo.Dhcp.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    sprintf(str, "DNS: %s", m_pSaleFunShareInfo->m_ShareNetInfo.Dns.toStdString().c_str());
    ATLib_u2g(str,strlen(str),data.str,64);
    m_pSaleFunShareInfo->m_pPrint->add(data);

    data.type = enLine;
    m_pSaleFunShareInfo->m_pPrint->add(data);

    QSqlQuery Calquery;
    int nRet = 0;
    if(nRet = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from TABLE_CALIBRATION where id=1;",&Calquery)>0)
    {
        Calquery.next();
        data.type = enString;
        sprintf(str, "%s", "标定信息");
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(str, "满量程: %s", Calquery.record().value(TABLE_CALIBRATION_FULLARRANGE).toString().toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(str, "标定量程: %s", Calquery.record().value(TABLE_CALIBRATION_CALARRANGE).toString().toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(data.str, "e: %s", Calquery.record().value(TABLE_CALIBRATION_E).toString().toStdString().c_str());
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(data.str, "e1: %s", Calquery.record().value(TABLE_CALIBRATION_E1).toString().toStdString().c_str());
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(str, "本地重力: %s", Calquery.record().value(TABLE_CALIBRATION_LOCALRAVITY).toString().toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(str, "目的地重力: %s", Calquery.record().value(TABLE_CALIBRATION_DEGRAVITY).toString().toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(str, "空载内码: %s", Calquery.record().value(TABLE_CALIBRATION_UNLOAD).toString().toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(str, "满载内码: %s", Calquery.record().value(TABLE_CALIBRATION_LOAD).toString().toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

        sprintf(str, "是否双量程: %s", Calquery.record().value(TABLE_CALIBRATION_ISDUAL).toString().toStdString().c_str());
        ATLib_u2g(str,strlen(str),data.str,64);
        m_pSaleFunShareInfo->m_pPrint->add(data);

    }
    Calquery.clear();

    data.type = enLine;
    m_pSaleFunShareInfo->m_pPrint->add(data);

    data.type = enString;
    QDateTime CurrentTime = QDateTime::currentDateTime();
    QString Timestr = CurrentTime.toString("yyyy-MM-dd hh:mm:ss");
    sprintf(data.str, "%s", Timestr.toStdString().c_str());
    m_pSaleFunShareInfo->m_pPrint->add(data);

    data.type = enQR;
    sprintf(data.str,"%s","www.cas.com.cnwefegdfghjkrtyuixcvbnmrtyuio3456756789ertyuiofghjklvbnm,dfghjklt");
    m_pSaleFunShareInfo->m_pPrint->add(data);

    m_pSaleFunShareInfo->m_pPrint->Print();

}
