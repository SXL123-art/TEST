#include "WebService.h"
#include "../ATSocket/cJSON.h"
#include "../ATSocket/CHttpClient.h"
#include "../ATSqlite/DBStruct.h"
#include <iostream>
#include <fstream>
#include <sstream>


int HeartBeat(WebPublic webPublic, CShareInfo *m_pInfo)
{
    m_pInfo->shareInfoSaleUi.ExternalWeb = false;

    QString deviceId = m_pInfo->m_ShareDeviceInfo.DeviceId;
    QString marketId =  m_pInfo->m_ShareDeviceInfo.MarketCode;

    QString JsonQStr = QString("{\"cmd\":8888,\"deviceId\":\"%1\",\"marketId\":\"%2\"}").arg(deviceId).arg(marketId);
    string curl = webPublic.weburl;
    string senddata = JsonQStr.toStdString();

    cout << "[url]:" << curl << endl;
    cout << "[SendData]:" << senddata << endl;

    CHttpClient client;
    string recvdata;
    int nTimeoutCnt = 0;
    while (1)
    {
        int nPostsResult = client.Posts(curl, senddata, recvdata, 0);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[GetTime]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 3)
            {
                nTimeoutCnt = 0;
                return -3;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            nTimeoutCnt = 0;
            return -4;
        }
        else
        {
            nTimeoutCnt = 0;
            return -5;
        }
        usleep(500*1000);
    }
    if (recvdata.length() <= 0)
        return -6;

    m_pInfo->shareInfoSaleUi.ExternalWeb = true;
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -1;
    }

    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(pJsonRoot, "returnState");
    if (itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get returnState faild !\n");
        return -1;
    }
    cout << "[returnState]:" << itemname->valueint << endl;
    if (itemname->valueint != 200)
    {
        itemname = cJSON_GetObjectItem(pJsonRoot, "message");
        if (itemname == NULL)
        {
            cJSON_Delete(pJsonRoot);
            printf("get message faild !\n");
            return -1;
        }
        cout << "[message]:" << itemname->valuestring << endl;
        cJSON_Delete(pJsonRoot);
        return -2;
    }

    itemname = cJSON_GetObjectItem(pJsonRoot, "results");
    if (itemname != NULL)
    {
        cJSON *item = cJSON_GetObjectItem(itemname, "cmd");
        if (item != NULL)
        {
            int cmd = itemname->valueint;
            if (cmd == 8002)
            {
                webPublic.userFlags = 1;
            }
            else if (cmd == 8003)
            {
                webPublic.pluFlags = 1;
            }
            cout << "[cmd]:" << cmd << endl;
        }
    }
    cJSON_Delete(pJsonRoot);
    return 1;
}

int GetServerTime(WebPublic webPublic,CShareInfo *m_pInfo)
{
    QString deviceId = m_pInfo->m_ShareDeviceInfo.DeviceId;
    QString marketId =  m_pInfo->m_ShareDeviceInfo.MarketCode;

    QString JsonQStr = QString("{\"cmd\":8001,\"deviceId\":\"%1\",\"marketId\":\"%2\"}").arg(deviceId).arg(marketId);
    string senddata = JsonQStr.toStdString();

    string curl = webPublic.weburl;
    cout << "[url]:" << curl << endl;
    cout << "[SendData]:" << senddata << endl;

    CHttpClient client;
    string recvdata;
    int nTimeoutCnt = 0;
    while (1)
    {
        int nPostsResult = client.Posts(curl, senddata, recvdata, 0);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[GetTime]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 3)
            {
                nTimeoutCnt = 0;
                return -3;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            nTimeoutCnt = 0;
            return -4;
        }
        else
        {
            nTimeoutCnt = 0;
            return -5;
        }
        usleep(500*1000);
    }
    if (recvdata.length() <= 0)
        return -6;

    //cout << "[recvdata]:" << recvdata;
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -1;
    }
    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(pJsonRoot, "returnState");
    if (itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get returnState faild !\n");
        return -1;
    }
    cout << "[returnState]:" << itemname->valueint << endl;
    if (itemname->valueint != 200)
    {
        itemname = cJSON_GetObjectItem(pJsonRoot, "message");
        if (itemname == NULL)
        {
            cJSON_Delete(pJsonRoot);
            printf("get message faild !\n");
            return -1;
        }
        cJSON_Delete(pJsonRoot);
        cout << "[message]:" << itemname->valuestring << endl;
        return -2;
    }

    itemname = cJSON_GetObjectItem(pJsonRoot, "time");
    if (itemname == NULL || itemname->valuestring == NULL || strlen(itemname->valuestring) <=10)
    {
        cJSON_Delete(pJsonRoot);
        printf("get time faild !\n");
        return -1;
    }
    cout << "[time]:" << itemname->valuestring << endl;

    char strTemp[32] = {0};
    sprintf(strTemp,"%s", itemname->valuestring);
    char strTime[32] = {0};
    memset(strTime,0x00,sizeof(strTime));
    sprintf(strTime, "%c%c%c%c%c%c%c%c%c%c%c%c", strTemp[2], strTemp[3], strTemp[5], strTemp[6],
            strTemp[8], strTemp[9], strTemp[11], strTemp[12], strTemp[14], strTemp[15], strTemp[17], strTemp[18]);
    printf("set strTime:%s\n",strTime);
    ATRtcWrite(strTime);				// 160405183045

    cJSON_Delete(pJsonRoot);
    return 1;
}

int GetUserInfo(WebPublic webPublic,CShareInfo *m_pInfo)
{
    QString deviceId = m_pInfo->m_ShareDeviceInfo.DeviceId;
    QString marketId =  m_pInfo->m_ShareDeviceInfo.MarketCode;

    QString JsonQStr = QString("{\"cmd\":8002,\"deviceId\":\"%1\",\"marketId\":\"%2\"}").arg(deviceId).arg(marketId);

    string curl = webPublic.weburl;
    string senddata = JsonQStr.toStdString();

    cout << "[url]:" << curl << endl;
    cout << "[SendData]:" << senddata << endl;

    CHttpClient client;
    string recvdata;
    int nTimeoutCnt = 0;
    while (1)
    {
        int nPostsResult = client.Posts(curl, senddata, recvdata, 0);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[GetTime]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 3)
            {
                nTimeoutCnt = 0;
                return -3;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            nTimeoutCnt = 0;
            return -4;
        }
        else
        {
            nTimeoutCnt = 0;
            return -5;
        }
        usleep(500*1000);
    }
    if (recvdata.length() <= 0)
        return -6;

    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -1;
    }
    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(pJsonRoot, "returnState");
    if (itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get returnState faild !\n");
        return -1;
    }
    cout << "[returnState]:" << itemname->valueint << endl;
    if (itemname->valueint != 200)
    {
        itemname = cJSON_GetObjectItem(pJsonRoot, "message");
        if (itemname != NULL)
        {
            cJSON_Delete(pJsonRoot);
            printf("get message faild !\n");
            return -1;
        }
        cJSON_Delete(pJsonRoot);
        cout << "[message]:" << itemname->valuestring << endl;
        return -2;
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "termversion");//电子秤版本号
    if (itemname != NULL)
    {
        if(strcmp(itemname->valuestring,SOFTWARE_VER) != 0)//直接调用升级程序接口
        {
            itemname = cJSON_GetObjectItem(pJsonRoot, "downloadurl");
            if (itemname != NULL)
            {
                cout << "[downloadurl]:" << itemname->valuestring << endl;
                // 升级标志置位并且启动升级程序
                std::string str = itemname->valuestring;
                sprintf(m_pInfo->shareInfoSaleUi.downloadUrl, "%s",str.c_str());
                int nPostsResult = client.url2file(str,"/root/Application/update.tar.bz2");
                if (nPostsResult == CURLE_OK)
                {
                    m_pInfo->ShowMessage("发现新的软件包，正在更新固件");
                    if (system("tar -xjvf /root/Application/update.tar.bz2") == 0)
                    {
                        if (system("/root/Application/update/update.sh") != 0)
                        {
                             printf("__________update.sh fail___________\n");
                        }
                        else
                        {
                            printf("__________update.sh success___________\n");
                            cJSON_Delete(pJsonRoot);
                            return -2;
                        }
                    }
                    else
                    {
                        printf("-----------------------tar -xjvf /root/Application/update.tar.bz2 fail\n");
                        memset(m_pInfo->shareInfoSaleUi.downloadUrl,0x00,LEN64);
                    }
                }
                cJSON_Delete(pJsonRoot);
                return 10;
            }
            cJSON_Delete(pJsonRoot);
            return -2;
        }
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "merchversion");//商户版本号
    if (itemname != NULL)
    {
        QString merchversion = itemname->valuestring;
        printf("old user ver:%s\n",m_pInfo->m_ShareDeviceInfo.ScaleNum.toStdString().c_str());
        printf("new user ver:%s\n",itemname->valuestring);
        if(m_pInfo->m_ShareDeviceInfo.ScaleNum != merchversion)
        {
            m_pInfo->m_ShareDeviceInfo.ScaleNum = merchversion;
            ATdb_Excute_SQL_Statements(QString("update TABLE_USERSETTING set value='%1' where id=17;").arg(merchversion));
            cJSON_Delete(pJsonRoot);
            return 11;
        }
    }

    itemname = cJSON_GetObjectItem(pJsonRoot, "wmode");//网络模式
    if (itemname != NULL)
    {
        QString sqlStr;
        sqlStr.sprintf("update table_scalesetting set value='%s' where id=7;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);

    }


    itemname = cJSON_GetObjectItem(pJsonRoot, "wname");//wifi名称
    if (itemname != NULL)
    {
        QString sqlStr;
        sqlStr.sprintf("update table_scalesetting set value='%s' where id=8;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);

    }

    itemname = cJSON_GetObjectItem(pJsonRoot, "wpassword");//wifi密码
    if (itemname != NULL)
    {
        QString sqlStr;
        sqlStr.sprintf("update table_scalesetting set value='%s' where id=9;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);

    }

//    itemname = cJSON_GetObjectItem(pJsonRoot, "logopicurl");//logo地址
//    if (itemname != NULL)
//    {
//        sprintf(m_pInfo->shareInfoSaleUi.picturlUrl,"%s",itemname->valuestring);
//    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "marketName");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.MarketName.sprintf("%s",itemname->valuestring);
        sprintf(m_pInfo->shareInfoSaleUi.systemName,"%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=19;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "marketId");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.MarketCode.sprintf("%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=18;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "operatorName");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.BoothName.sprintf("%s",itemname->valuestring);
        sprintf(m_pInfo->shareInfoSaleUi.sellerName,"商户:%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=15;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }

    itemname = cJSON_GetObjectItem(pJsonRoot, "scaleType");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.DeviceType.sprintf("%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=13;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "boothId");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.BoothId.sprintf("%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=14;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "discount");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.BoothCode.sprintf("%d",itemname->valueint);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%d' where id=16;", itemname->valueint);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "traceAddress");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.HeadTail5.sprintf("%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=24;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "paperTitel");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.HeadTail6.sprintf("%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=25;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "headTail1");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.HeadTail1.sprintf("%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=20;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "headTail2");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.HeadTail2.sprintf("%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=21;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    itemname = cJSON_GetObjectItem(pJsonRoot, "headTail3");
    if (itemname != NULL)
    {
        m_pInfo->m_ShareDeviceInfo.HeadTail3.sprintf("%s",itemname->valuestring);
        QString sqlStr;
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=22;", itemname->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    cJSON_Delete(pJsonRoot);
    return 1;
}

int GetPluInfo(WebPublic webPublic,CShareInfo *m_pInfo)
{
    QString deviceId = m_pInfo->m_ShareDeviceInfo.DeviceId;
    QString marketId =  m_pInfo->m_ShareDeviceInfo.MarketCode;

    QString JsonQStr = QString("{\"cmd\":8003,\"deviceId\":\"%1\",\"marketId\":\"%2\"}").arg(deviceId).arg(marketId);
    string curl = webPublic.weburl;
    string senddata = JsonQStr.toStdString();

    cout << "[url]:" << curl << endl;
    cout << "[SendData]:" << senddata << endl;

    CHttpClient client;
    string recvdata;
    int nTimeoutCnt = 0;
    while (1)
    {
        int nPostsResult = client.Posts(curl, senddata, recvdata, 0);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[GetTime]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 3)
            {
                nTimeoutCnt = 0;
                return -3;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            nTimeoutCnt = 0;
            return -4;
        }
        else
        {
            nTimeoutCnt = 0;
            return -5;
        }
        usleep(500*1000);
    }
    if (recvdata.length() <= 0)
        return -6;

    //cout << "[recvdata]:" << recvdata;
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -1;
    }

    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(pJsonRoot, "returnState");
    if (itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get returnState faild !\n");
        return -1;
    }
    cout << "[returnState]:" << itemname->valueint << endl;
    if (itemname->valueint != 200)
    {
        itemname = cJSON_GetObjectItem(pJsonRoot, "message");
        if (itemname != NULL)
        {
            cout << "[message]:" << itemname->valuestring << endl;
        }
        cJSON_Delete(pJsonRoot);
        cout << "[returnState]:" << itemname->valueint << endl;
        return -1;
    }
    cJSON *pluListArry = cJSON_GetObjectItem(pJsonRoot, "PluList");
    if(NULL == pluListArry)
    {
        cout << "[PluList]:" << "is nil" << endl;
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    int pluListSize = cJSON_GetArraySize(pluListArry);
    if(pluListSize <= 0)
    {
        cout << "[pluListSize]:" << "0" << endl;
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    printf("-------pluListSize:%d\n", pluListSize);
    ATdb_Excute_SQL_Statements("delete from table_product;");
    ATdb_Excute_SQL_Statements("delete from table_convert;");
    struct_tb_product *plu= new struct_tb_product[pluListSize];
    struct_tb_convert *key= new struct_tb_convert[pluListSize];
    cJSON *itemInfo = NULL;
    cJSON *arrayitem = NULL;
    for(int i = 0; i<pluListSize;i++)
    {
        arrayitem = cJSON_GetArrayItem(pluListArry, i);
        itemInfo = cJSON_GetObjectItem(arrayitem, "goodsCode");
        if(itemInfo != NULL)	//必填项
        {
            printf("goodsCode:%s\n",itemInfo->valuestring);
            plu[i].goodsCode.sprintf("%s" ,itemInfo->valuestring);
            key[i].goodsCode.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(arrayitem, "goodsName");
        if(itemInfo != NULL)	//必填项
        {
            printf("goodsName:%s\n",itemInfo->valuestring);
            plu[i].goodsName.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(arrayitem, "goodsKey");
        if(itemInfo != NULL)	//必填项
        {
            printf("goodsKey:%s\n",itemInfo->valuestring);
            key[i].hotKey.sprintf("%s" ,itemInfo->valuestring);
            plu[i].goodsKey.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(arrayitem, "goodsPrice");
        if(itemInfo != NULL)
        {
            printf("goodsPrice:%d\n",itemInfo->valueint);
            plu[i].lastPrice.sprintf("%d" ,itemInfo->valueint);
        }
        itemInfo = cJSON_GetObjectItem(arrayitem, "salesMode");
        if(itemInfo != NULL)
        {
            printf("salesMode:%s\n",itemInfo->valuestring);
            plu[i].isPcs.sprintf("%s" ,itemInfo->valuestring);
        }
    }

    ATdb_Insert_DataList_By_TableName_DataList(TABLE_PRODUCT, plu, pluListSize);
    ATdb_Insert_DataList_By_TableName_DataList(TABLE_CONVERT, key, pluListSize);
    delete[] plu;   
    delete[] key;
    cJSON_Delete(pJsonRoot);
    printf("Get Plu===================End\n");
    return pluListSize;
}

int UploadSaleInfo(WebPublic webPublic,CShareInfo *m_pInfo)
{
    QSqlQuery AttachQuery;
    int AttachSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from TABLE_ATTACH where webUp='0' order by id desc limit 1;",&AttachQuery);
    if (AttachSize <= 0)
    {
        printf(" TABLE_ATTACH no data or data error---------------------------\n");
        AttachQuery.clear();
        return 0;
    }

    AttachQuery.next();
    QString businessCode = AttachQuery.record().value(TABLE_ATTACH_BUSINESSCODE).toString();
    QString serialNumber = AttachQuery.record().value(TABLE_ATTACH_SERIAL).toString();
    QString count = AttachQuery.record().value(TABLE_ATTACH_SALENUM).toString();
    QString amount = AttachQuery.record().value(TABLE_ATTACH_SUBTOTAL).toString();
    QString payMoney = AttachQuery.record().value(TABLE_ATTACH_PAYMONEY).toString();
    QString payType = AttachQuery.record().value(TABLE_ATTACH_PAYTYPE).toString();
    QString tradeTime = AttachQuery.record().value(TABLE_ATTACH_SCALETIME).toString();
    QString discount = AttachQuery.record().value(TABLE_ATTACH_CONTEXT).toString();
    if(discount.length()<=0 || discount.toInt() >100 )discount = "100";
    QString saleTotalTable = QString("{\"serialNumber\":\"%1\", \"count\":%2,\"discount\":%3, \"amount\":\"%4\", \"payMoney\":\"%5\", \"payType\":%6, \"tradeTime\":\"%7\"}")
            .arg(serialNumber).arg(count).arg(discount).arg(amount).arg(payMoney).arg(payType).arg(tradeTime);

    QSqlQuery BusinessQuery;
    int BusinessSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(QString("select * from TABLE_BUSINESS where  businessCode=%1 ;").arg(businessCode),&BusinessQuery);
    if(BusinessSize <= 0)
    {
        ATdb_Excute_SQL_Statements(QString("delete from TABLE_ATTACH where businessCode=%1 ;").arg(businessCode));
        printf("TABLE_BUSINESS no data or data error---------------------------无交易数据\n");
        AttachQuery.clear();
        BusinessQuery.clear();
        return 0;
    }

    QString deviceId = m_pInfo->m_ShareDeviceInfo.DeviceId;
    QString marketId =  m_pInfo->m_ShareDeviceInfo.MarketCode;

    QString saleDetailTable = "[";
    for(int i = 0 ;i < BusinessSize; i++)
    {
        BusinessQuery.next();

        QString goodsCode = BusinessQuery.record().value(TABLE_BUSINESS_GOODSCODE).toString();
        QString goodsName = BusinessQuery.record().value(TABLE_BUSINESS_GOODSNAME).toString();
        QString goodsPrice = BusinessQuery.record().value(TABLE_BUSINESS_PRICE).toString();
        QString totalPrice = BusinessQuery.record().value(TABLE_BUSINESS_TOTAL).toString();
        QString salesMode = BusinessQuery.record().value(TABLE_BUSINESS_SALEMODE).toString();
        QString goodsWeight = "";
        if(salesMode.compare("1") == 0)
        {
            goodsWeight = BusinessQuery.record().value(TABLE_BUSINESS_PIECES).toString();
        }
        else
        {
            goodsWeight = BusinessQuery.record().value(TABLE_BUSINESS_WEIGHT).toString();
        }
        QString salesTime = BusinessQuery.record().value(TABLE_BUSINESS_TIME).toString();
        QString temp = QString("{\"goodsCode\":\"%1\", \"goodsName\":\"%2\", \"goodsWeight\":%3, \"goodsPrice\":%4, \"totalPrice\":%5, \"salesMode\":\"%6\", \"salesTime\":\"%7\"}")
                               .arg(goodsCode).arg(goodsName).arg(goodsWeight).arg(goodsPrice).arg(totalPrice).arg(salesMode).arg(salesTime);

        saleDetailTable.append(temp);
        saleDetailTable.append(",");
    }
    saleDetailTable = saleDetailTable.left(saleDetailTable.length() - 1);
    saleDetailTable +="]";

    QString JsonQStr = QString("{\"cmd\":8088,\"deviceId\":\"%1\",\"marketId\":\"%2\", \"saleTotalTable\":%5, \"saleDetailTable\":%6}")
                       .arg(deviceId).arg(marketId).arg(saleTotalTable).arg(saleDetailTable);

    string curl = webPublic.weburl;// + "/uploadOrder";
    string senddata = JsonQStr.toStdString();

    cout << "[url]:" << curl << endl;
    cout << "[SendData]:" << senddata << endl;

    string recvdata;
    int nTimeoutCnt = 0;
    CHttpClient client;
    while (1)
    {
        int nPostsResult = client.Posts(curl, senddata, recvdata, NULL);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[UploadOrder]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 3)
            {
                nTimeoutCnt = 0;
                return -3;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            nTimeoutCnt = 0;
            return -4;
        }
        else
        {
            nTimeoutCnt = 0;
            return -5;
        }
        usleep(500*1000);
    }
    if (recvdata.length() <= 0)
    return -6;

    //cout << "[recvdata]:" << recvdata;
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -1;
    }
    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(pJsonRoot, "returnState");
    if (itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    cout << "[returnState]:" << itemname->valueint << endl;
    if (itemname->valueint != 200)
    {
        itemname = cJSON_GetObjectItem(pJsonRoot, "message");
        if (itemname != NULL)
        {
            cout << "[msg]:" << itemname->valuestring << endl;
        }

        ATdb_Excute_SQL_Statements(QString("update TABLE_ATTACH set webUp='100' where businessCode='%1';").arg(businessCode));
        ATdb_Excute_SQL_Statements(QString("update TABLE_BUSINESS set webUp='100' where businessCode='%1' and webUp='0';").arg(businessCode));
        cJSON_Delete(pJsonRoot);
        cout << "[returnState]:" << itemname->valueint << endl;
        return -2;
    }
    ATdb_Excute_SQL_Statements(QString("update TABLE_ATTACH set webUp='1' where businessCode='%1';").arg(businessCode));
    ATdb_Excute_SQL_Statements(QString("update TABLE_BUSINESS set webUp='1' where businessCode='%1' and webUp='0';").arg(businessCode));
    cJSON_Delete(pJsonRoot);
    return 1;
}
