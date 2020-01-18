#include "JNLC_WebService.h"

#include "../ATSocket/cJSON.h"
#include "../ATSocket/CHttpClient.h"
#include "../ATSqlite/DBStruct.h"

#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <sstream>

int JNLC_GetServerSystemTime(JNLC_WebPublic webPublic,CShareInfo *m_pInfo)
{
    //1
    QString BIZ_TYPE = "getSysTime";
    //2
    QDateTime time = QDateTime::currentDateTime();                              //获取系统现在的时间
    QString REQ_TIME = time.toString("yyyy-MM-dd hh:mm:ss");
    //3
    char strRand[6] = {0x00};
    getRandStr(strRand, 6);
    QString REQ_ID = time.toString("yyyyMMddhhmmss") + strRand;
    //4
    QString AUTH_ID =  QString::fromStdString(webPublic.auth_id);
    //5
    QString PARAM = "{}";

    QString JsonQStr = "";
    JsonQStr = QString("{\"BIZ_TYPE\":\"%1\",\"REQ_TIME\":\"%2\",\"REQ_ID\":\"%3\",\"AUTH_ID\":\"%4\",\"PARAM\":%5}")
            .arg(BIZ_TYPE).arg(REQ_TIME).arg(REQ_ID).arg(AUTH_ID).arg(PARAM);

    // 生成 SHA256 散列值
    unsigned char sha256[SHA256_DIGEST_LENGTH];
    memset(sha256, 0x00, SHA256_DIGEST_LENGTH);

    QString temp = JsonQStr + QString::fromStdString(webPublic.key);
    SHA256((unsigned char *) temp.toStdString().c_str(), temp.toStdString().length(),sha256);
    // 转换成16进制str
    char sha256Hex[SHA256_DIGEST_LENGTH*2];
    memset(sha256Hex, 0x00, SHA256_DIGEST_LENGTH*2);
    BcdToStr(sha256Hex,(char*)sha256,SHA256_DIGEST_LENGTH);

    JsonQStr.append(sha256Hex);
    //qDebug()<<"JsonQStr:"<<JsonQStr<<endl;

    string senddata = JsonQStr.toStdString();
    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Post(webPublic.WebUrl, senddata, recvdata);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[getSysTime]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 0)
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

    if(recvdata.length() <= 0)
    {
        return -6;
    }

    int start = recvdata.find_first_of('{');
    int end = recvdata.find_last_of('}');
    cout << "[start]:"<<start <<"---[end]:"<< end << endl;
    if(start == end || end <=0)
    {
        return -7;
        qDebug()<<"---------------recvdata is not JSON"<<endl;
    }

    recvdata = recvdata.substr(start, end+1);
    cout << "recvdata:"<<recvdata << endl;

    //------------------------------------------------------------解析
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -8;
    }

    cJSON *RESULTJson = NULL;
    RESULTJson = cJSON_GetObjectItem(pJsonRoot, "RESULT");
    if(RESULTJson == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT faild !\n");
        return -8;
    }

    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(RESULTJson, "RESULT_CODE");
    if(itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT_CODE faild !\n");
        return -8;
    }

    if(strcmp(itemname->valuestring,"0000")==1) //相等返回0
    {
        cout << "2---[RESULT_CODE]:"<<itemname->valuestring<<endl;
        cJSON_Delete(pJsonRoot);
        return -8;
    }
    cout << "1---[RESULT_CODE]:"<<itemname->valuestring<<endl;

    itemname = cJSON_GetObjectItem(RESULTJson, "SYS_TIME");
    if(itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get SYS_TIME faild !\n");
        return -8;
    }
    cout << "[SYS_TIME]:"<<itemname->valuestring<<endl;

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
int JNLC_GetGoodsList(JNLC_WebPublic webPublic,CShareInfo *m_pInfo)
{
    //1
    QString BIZ_TYPE = "getGoodsList";
    //2
    QDateTime time = QDateTime::currentDateTime();                              //获取系统现在的时间
    QString REQ_TIME = time.toString("yyyy-MM-dd hh:mm:ss");
    //3
    char strRand[6] = {0x00};
    getRandStr(strRand, 6);
    QString REQ_ID = time.toString("yyyyMMddhhmmss") + strRand;
    //4
    QString AUTH_ID =  QString::fromStdString(webPublic.auth_id);
    //5
    QString PARAM = "";
    QString BUSI_CODE = m_pInfo->m_ShareDeviceInfo.BoothCode;
    QString VARIETY_TYPE = m_pInfo->m_ShareDeviceInfo.DeviceType;
    PARAM = QString("{\"BUSI_CODE\":\"%1\",\"VARIETY_TYPE\":\"%2\"}")
            .arg(BUSI_CODE).arg(VARIETY_TYPE);

    QString JsonQStr = "";
    JsonQStr = QString("{\"BIZ_TYPE\":\"%1\",\"REQ_TIME\":\"%2\",\"REQ_ID\":\"%3\",\"AUTH_ID\":\"%4\",\"PARAM\":%5}")
            .arg(BIZ_TYPE).arg(REQ_TIME).arg(REQ_ID).arg(AUTH_ID).arg(PARAM);

    // 生成 SHA256 散列值
    unsigned char sha256[SHA256_DIGEST_LENGTH];
    memset(sha256, 0x00, SHA256_DIGEST_LENGTH);

    QString temp = JsonQStr + QString::fromStdString(webPublic.key);
    SHA256((unsigned char *) temp.toStdString().c_str(), temp.toStdString().length(),sha256);
    // 转换成16进制str
    char sha256Hex[SHA256_DIGEST_LENGTH*2];
    memset(sha256Hex, 0x00, SHA256_DIGEST_LENGTH*2);
    BcdToStr(sha256Hex,(char*)sha256,SHA256_DIGEST_LENGTH);

    JsonQStr.append(sha256Hex);
    //qDebug()<<"JsonQStr:"<<JsonQStr<<endl;

    string senddata = JsonQStr.toStdString();
    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Post(webPublic.WebUrl, senddata, recvdata);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[getGoodsList]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 0)
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

    if(recvdata.length() <= 0)
    {
        return -6;
    }

    int start = recvdata.find_first_of('{');
    int end = recvdata.find_last_of('}');
    cout << "[start]:"<<start <<"---[end]:"<< end << endl;
    if(start == end || end <=0)
    {
        return -7;
        qDebug()<<"---------------recvdata is not JSON"<<endl;
    }

    recvdata = recvdata.substr(start, end+1);
    //cout << "recvdata:"<<recvdata << endl;

    //---------------------------------------------------------------解析
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -8;
    }

    cJSON *RESULTJson = NULL;
    RESULTJson = cJSON_GetObjectItem(pJsonRoot, "RESULT");
    if(RESULTJson == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT faild !\n");
        return -8;
    }

    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(RESULTJson, "RESULT_CODE");
    if(itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT_CODE faild !\n");
        return -8;
    }
    if(strcmp(itemname->valuestring,"0000")==1) //相等返回0
    {
        cout << "2---[RESULT_CODE]:"<<itemname->valuestring<<endl;
        cJSON_Delete(pJsonRoot);
        return -8;
    }
    cout << "1---[RESULT_CODE]:"<<itemname->valuestring<<endl;

    //------------GOODS_LIST
    cJSON *List = NULL;
    List = cJSON_GetObjectItem(RESULTJson, "GOODS_LIST");
    if(List == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get GOODS_LIST faild !\n");
        return -8;
    }
    int listSize = cJSON_GetArraySize(List);
    printf("-------[GOODS_LIST listSize]:%d\n", listSize);
    if (listSize > 0)
    {
        struct_tb_product *plu= new struct_tb_product[listSize];
        struct_tb_convert *key= new struct_tb_convert[listSize];
        cJSON *arrayitem = NULL;
        cJSON *itemname = NULL;
        for(int i = 0; i<listSize;i++)
        {
            printf("------------[%d]\n",i+1);
            plu[i].ClearAllData();
            key[i].ClearAllData();

            arrayitem = cJSON_GetArrayItem(List, i);
            itemname = cJSON_GetObjectItem(arrayitem, "VARIETY_CODE");
            if(itemname != NULL)
            {
                plu[i].goodsCode = QString("%1").arg(itemname->valuestring);
                key[i].goodsCode = QString("%1").arg(itemname->valuestring);
                printf("[VARIETY_CODE]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "VARIETY_NAME");
            if(itemname != NULL)
            {
                plu[i].goodsName = QString("%1").arg(itemname->valuestring);
                printf("[VARIETY_NAME]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "VARIETY_TYPE");
            if(itemname != NULL)
            {
                plu[i].groupNo = QString("%1").arg(itemname->valuestring);
                printf("[VARIETY_TYPE]:%s\n",itemname->valuestring);
            }
            plu[i].goodsKey = QString("%1").arg(i+1);
            key[i].hotKey = QString("%1").arg(i+1);
        }
        ATdb_Excute_SQL_Statements("delete from table_product;");
        ATdb_Excute_SQL_Statements("delete from table_convert;");
        ATdb_Insert_DataList_By_TableName_DataList(TABLE_PRODUCT,plu,listSize);
        ATdb_Insert_DataList_By_TableName_DataList(TABLE_CONVERT,key,listSize);
        delete[] key;
        delete[] plu;
    }
    cJSON_Delete(pJsonRoot);
    return 1;
}

int JNLC_EnterDataToScales(JNLC_WebPublic webPublic,CShareInfo *m_pInfo)
{
    //1
    QString BIZ_TYPE = "getInData";
    //2
    QDateTime time = QDateTime::currentDateTime();                              //获取系统现在的时间
    QString REQ_TIME = time.toString("yyyy-MM-dd hh:mm:ss");
    //3
    char strRand[6] = {0x00};
    getRandStr(strRand, 6);
    QString REQ_ID = time.toString("yyyyMMddhhmmss") + strRand;
    //4
    QString AUTH_ID =  QString::fromStdString(webPublic.auth_id);
    //5
    QString PARAM = "";
    QString BUSI_CODE = m_pInfo->m_ShareDeviceInfo.BoothCode;
    QString VARIETY_TYPE = m_pInfo->m_ShareDeviceInfo.DeviceType;
    QString IN_DATE = time.toString("yyyy-MM-dd");
    PARAM = QString("{\"BUSI_CODE\":\"%1\",\"VARIETY_TYPE\":\"%2\",\"IN_DATE\":\"%3\"}")
            .arg(BUSI_CODE).arg(VARIETY_TYPE).arg(IN_DATE);

    QString JsonQStr = "";
    JsonQStr = QString("{\"BIZ_TYPE\":\"%1\",\"REQ_TIME\":\"%2\",\"REQ_ID\":\"%3\",\"AUTH_ID\":\"%4\",\"PARAM\":%5}")
            .arg(BIZ_TYPE).arg(REQ_TIME).arg(REQ_ID).arg(AUTH_ID).arg(PARAM);

    // 生成 SHA256 散列值
    unsigned char sha256[SHA256_DIGEST_LENGTH];
    memset(sha256, 0x00, SHA256_DIGEST_LENGTH);

    QString temp = JsonQStr + QString::fromStdString(webPublic.key);
    SHA256((unsigned char *) temp.toStdString().c_str(), temp.toStdString().length(),sha256);
    // 转换成16进制str
    char sha256Hex[SHA256_DIGEST_LENGTH*2];
    memset(sha256Hex, 0x00, SHA256_DIGEST_LENGTH*2);
    BcdToStr(sha256Hex,(char*)sha256,SHA256_DIGEST_LENGTH);

    JsonQStr.append(sha256Hex);
    //qDebug()<<"JsonQStr:"<<JsonQStr<<endl;

    string senddata = JsonQStr.toStdString();
    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Post(webPublic.WebUrl, senddata, recvdata);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[getInData]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 0)
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

    if(recvdata.length() <= 0)
    {
        return -6;
    }

    int start = recvdata.find_first_of('{');
    int end = recvdata.find_last_of('}');
    cout << "[start]:"<<start <<"---[end]:"<< end << endl;
    if(start == end || end <=0)
    {
        return -7;
        qDebug()<<"---------------recvdata is not JSON"<<endl;
    }

    recvdata = recvdata.substr(start, end+1);
    //cout << "recvdata:"<<recvdata << endl;

    //---------------------------------------------------------------解析
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -8;
    }

    cJSON *RESULTJson = NULL;
    RESULTJson = cJSON_GetObjectItem(pJsonRoot, "RESULT");
    if(RESULTJson == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT faild !\n");
        return -8;
    }

    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(RESULTJson, "RESULT_CODE");
    if(itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT_CODE faild !\n");
        return -8;
    }
    if(strcmp(itemname->valuestring,"0000")==1) //相等返回0
    {
        cout << "2---[RESULT_CODE]:"<<itemname->valuestring<<endl;
        cJSON_Delete(pJsonRoot);
        return -8;
    }
    cout << "1---[RESULT_CODE]:"<<itemname->valuestring<<endl;

    //------------JsonList
    cJSON *List = NULL;
    List = cJSON_GetObjectItem(RESULTJson, "IN_DATA");
    if(List == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get IN_DATA JsonList faild !\n");
        return -8;
    }
    int listSize = cJSON_GetArraySize(List);
    printf("-------[IN_DATA listSize]:%d\n", listSize);
    if (listSize > 0)
    {
        struct_tb_batch *pluDetail = new struct_tb_batch[1];
        cJSON *arrayitem = NULL;
        cJSON *itemname = NULL;
        QString GoodsPrice = "";
        QString sqlStr = "";

        for(int i = 0; i<listSize;i++)
        {
            pluDetail[0].ClearAllData();
            GoodsPrice.clear();
            arrayitem = cJSON_GetArrayItem(List, i);
            itemname = cJSON_GetObjectItem(arrayitem, "IN_DATE");
            if(itemname != NULL)
            {
                pluDetail[0].time = itemname->valuestring;
                printf("[IN_DATE]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "BUSI_CODE");
            if(itemname != NULL)
            {
                pluDetail[0].batchY = itemname->valuestring;
                printf("[BUSI_CODE]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "BUSI_NAME");
            if(itemname != NULL)
            {
                pluDetail[0].batchZ = itemname->valuestring;
                printf("[BUSI_NAME]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "PRE_TRAN_ID");
            if(itemname != NULL)
            {
                pluDetail[0].traceCode = itemname->valuestring;
                printf("[PRE_TRAN_ID]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "BATCH_ID");
            if(itemname != NULL)
            {
                pluDetail[0].batchCode = itemname->valuestring;
                printf("[BATCH_ID]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "VARIETY_CODE");
            if(itemname != NULL)
            {
                pluDetail[0].goodsCode = itemname->valuestring;
                printf("[VARIETY_CODE]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "VARIETY_NAME");
            if(itemname != NULL)
            {
                pluDetail[0].goodsName = itemname->valuestring;
                printf("[VARIETY_NAME]:%s\n",itemname->valuestring);
            }

            itemname = cJSON_GetObjectItem(arrayitem, "VARIETY_TYPE");
            if(itemname != NULL)
            {
                pluDetail[0].groupNo = itemname->valuestring;
                printf("[VARIETY_TYPE]:%s\n",itemname->valuestring);

            }

//            itemname = cJSON_GetObjectItem(arrayitem, "WEIGHT");
//            if(itemname != NULL)
//            {
//                printf("[WEIGHT]:%s\n",itemname->valuestring);
//            }

            itemname = cJSON_GetObjectItem(arrayitem, "PRICE");
            if(itemname != NULL)
            {
                GoodsPrice = QString("'%1'").arg((int)(atof(itemname->valuestring) *100.0f + 0.01));
                printf("[PRICE]:%s\n",itemname->valuestring);
                if(GoodsPrice.length ()>0)
                {
                    sqlStr = QString("update TABLE_PRODUCT set lastprice=%1 where  goodsCode='%2';").arg(GoodsPrice).arg(pluDetail[0].goodsCode);
                    ATdb_Excute_SQL_Statements(sqlStr);
                }
            }
            if(pluDetail[0].groupNo == "RL")
            {
                ATdb_Excute_SQL_Statements("delete from TABLE_BATCH;");
            }
            else
            {
                /*sqlStr = QString("select count(*) from TABLE_BATCH where batchCode='%1' and goodsCode='%2';").arg(pluDetail[0].batchCode).arg(pluDetail[0].goodsCode);
                QString returnStr;
                ATdb_Select_ReturnQString_By_SQL_Statements(sqlStr,&returnStr);
                bool ok;
                if(returnStr.toInt(&ok,10) > 0)
                {
                    //如果电子秤存在该进场就丢掉
                    qDebug()<<"-----------------The batch code already exists"<<endl;
                    continue;
                }*/
                sqlStr = QString("delete from TABLE_BATCH where goodsCode='%2';").arg(pluDetail[0].goodsCode);
                ATdb_Excute_SQL_Statements(sqlStr);
            }
            ATdb_Insert_DataList_By_TableName_DataList(TABLE_BATCH,pluDetail,1);

        }
        delete[] pluDetail;
    }
    cJSON_Delete(pJsonRoot);
    return 1;
}

int JNLC_GetTraceCode(JNLC_WebPublic webPublic,CShareInfo *m_pInfo)
{
    //1
    QString BIZ_TYPE = "getTraceCode";
    //2
    QDateTime time = QDateTime::currentDateTime();                              //获取系统现在的时间
    QString REQ_TIME = time.toString("yyyy-MM-dd hh:mm:ss");
    //3
    char strRand[6] = {0x00};
    getRandStr(strRand, 6);
    QString REQ_ID = time.toString("yyyyMMddhhmmss") + strRand;
    //4
    QString AUTH_ID =  QString::fromStdString(webPublic.auth_id);
    //5
    QString PARAM = "";
    QString BUSI_CODE = m_pInfo->m_ShareDeviceInfo.BoothCode;
    QString COUNT = QString("%1").arg(m_pInfo->m_nSaleNum);          //0～100
    PARAM = QString("{\"BUSI_CODE\":\"%1\",\"COUNT\":\"%2\"}")
            .arg(BUSI_CODE).arg(COUNT);

    QString JsonQStr = "";
    JsonQStr = QString("{\"BIZ_TYPE\":\"%1\",\"REQ_TIME\":\"%2\",\"REQ_ID\":\"%3\",\"AUTH_ID\":\"%4\",\"PARAM\":%5}")
            .arg(BIZ_TYPE).arg(REQ_TIME).arg(REQ_ID).arg(AUTH_ID).arg(PARAM);

    // 生成 SHA256 散列值
    unsigned char sha256[SHA256_DIGEST_LENGTH];
    memset(sha256, 0x00, SHA256_DIGEST_LENGTH);

    QString temp = JsonQStr + QString::fromStdString(webPublic.key);
    SHA256((unsigned char *) temp.toStdString().c_str(), temp.toStdString().length(),sha256);
    // 转换成16进制str
    char sha256Hex[SHA256_DIGEST_LENGTH*2];
    memset(sha256Hex, 0x00, SHA256_DIGEST_LENGTH*2);
    BcdToStr(sha256Hex,(char*)sha256,SHA256_DIGEST_LENGTH);

    JsonQStr.append(sha256Hex);
    //qDebug()<<"JsonQStr:"<<JsonQStr<<endl;

    string senddata = JsonQStr.toStdString();
    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Post(webPublic.WebUrl, senddata, recvdata);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[getTraceCode]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 0)
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

    if(recvdata.length() <= 0)
    {
        return -6;
    }

    int start = recvdata.find_first_of('{');
    int end = recvdata.find_last_of('}');
    cout << "[start]:"<<start <<"---[end]:"<< end << endl;
    if(start == end || end <=0)
    {
        return -7;
        qDebug()<<"---------------recvdata is not JSON"<<endl;
    }

    recvdata = recvdata.substr(start, end+1);
    //cout << "recvdata:"<<recvdata << endl;

    //---------------------------------------------------------------解析
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -8;
    }

    cJSON *RESULTJson = NULL;
    RESULTJson = cJSON_GetObjectItem(pJsonRoot, "RESULT");
    if(RESULTJson == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT faild !\n");
        return -8;
    }

    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(RESULTJson, "RESULT_CODE");
    if(itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT_CODE faild !\n");
        return -8;
    }
    if(strcmp(itemname->valuestring,"0000")==1) //相等返回0
    {
        cout << "2---[RESULT_CODE]:"<<itemname->valuestring<<endl;
        cJSON_Delete(pJsonRoot);
        return -8;
    }
    cout << "1---[RESULT_CODE]:"<<itemname->valuestring<<endl;

    //------------JsonList
    cJSON *List = NULL;
    List = cJSON_GetObjectItem(RESULTJson, "TRACE_CODES");
    if(List == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get TRACE_CODES JsonList faild !\n");
        return -8;
    }
    //printf("-------[listSize]:%s\n", List->valuestring);
    int listSize = cJSON_GetArraySize(List);
    printf("-------[TRACE_CODES listSize]:%d\n", listSize);
    if (listSize > 0)
    {
        cJSON *arrayitem = NULL;
        for(int i = 0; i<listSize;i++)
        {
            arrayitem = cJSON_GetArrayItem(List, i);
            if(arrayitem != NULL)
            {
               ;
            }
        }
    }
    cJSON_Delete(pJsonRoot);
    return 1;
}

int JNLC_PutTransData(JNLC_WebPublic webPublic,CShareInfo *m_pInfo)
{
    QSqlQuery AttachQuery;
    int AttachSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from TABLE_ATTACH where  webUp='0' order by id desc limit 1;",&AttachQuery);
    if (AttachSize <= 0)
    {
        printf(" TABLE_ATTACH no data or data error---------------------------\n");
        AttachQuery.clear();
        return 0;
    }
    AttachQuery.next();
    QString businessCode = AttachQuery.record().value(TABLE_ATTACH_BUSINESSCODE).toString();
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
    unsigned char batchflag = 0;        //记录有批次码的交易数
    QString TRAN_DATA = "[";
    for(int i = 0 ;i < BusinessSize; i++)
    {
        BusinessQuery.next();
        QString batchcode = BusinessQuery.record().value(TABLE_BUSINESS_BATCHCODE).toString();
        QString goodsCode = BusinessQuery.record().value(TABLE_BUSINESS_GOODSCODE).toString();
        qDebug() << "batchcode: " << batchcode;
        qDebug() << "goodsCode: " << goodsCode;
        if(batchcode.length() <=2 || goodsCode.length()<=0)  //去除无批次码交易,无商品号
        {
            printf("TABLE_BUSINESS BusinessCode:%s---SerialNum:%s no batch code---------------------------\n",
                    BusinessQuery.record().value(TABLE_BUSINESS_BUSINESSCODE).toString().toStdString().c_str()
                   ,BusinessQuery.record().value(TABLE_BUSINESS_SERIALNUM).toString().toStdString().c_str());
            ATdb_Excute_SQL_Statements(QString("update TABLE_BUSINESS set webUp='2' where businessCode='%1' and serialNum='%2';")
                                       .arg(BusinessQuery.record().value(TABLE_BUSINESS_BUSINESSCODE).toString())
                                       .arg(BusinessQuery.record().value(TABLE_BUSINESS_SERIALNUM).toString()));
            continue;
        }
        batchflag++;
        QString SALE_DATE = BusinessQuery.record().value(TABLE_BUSINESS_TIME).toString().section(" ",0,0);
        QString SELLER_BUSI_CODE =  m_pInfo->m_ShareDeviceInfo.BoothCode;
        QString SELLER_BUSI_NAME =  m_pInfo->m_ShareDeviceInfo.BoothName;
        QString BATCH_ID = BusinessQuery.record().value(TABLE_BUSINESS_BATCHCODE).toString();
        QString TRAN_ID = BusinessQuery.record().value(TABLE_BUSINESS_TRACECODE).toString();
        QString VARIETY_CODE = BusinessQuery.record().value(TABLE_BUSINESS_GOODSCODE).toString();
        QString VARIETY_NAME = BusinessQuery.record().value(TABLE_BUSINESS_GOODSNAME).toString();
        QString VARIETY_TYPE = m_pInfo->m_ShareDeviceInfo.DeviceType;
        QString mode = BusinessQuery.record().value(TABLE_BUSINESS_SALEMODE).toString();
        QString WEIGHT = "";
        if(mode.compare("1") == 0)
        {
            WEIGHT = BusinessQuery.record().value(TABLE_BUSINESS_PIECES).toString();
        }
        else
        {
            WEIGHT = BusinessQuery.record().value(TABLE_BUSINESS_WEIGHT).toString();
        }

        QString PRICE = BusinessQuery.record().value(TABLE_BUSINESS_PRICE).toString();
        QString temp = "";
        temp = QString("{\"SALE_DATE\":\"%1\",\"SELLER_BUSI_CODE\":\"%2\",\"SELLER_BUSI_NAME\":\"%3\",\"BATCH_ID\":\"%4\",\"TRAN_ID\":\"%5\","
                       "\"VARIETY_CODE\":\"%6\",\"VARIETY_NAME\":\"%7\",\"VARIETY_TYPE\":\"%8\",\"WEIGHT\":\"%9\",\"PRICE\":\"%10\"}")
                .arg(SALE_DATE).arg(SELLER_BUSI_CODE).arg(SELLER_BUSI_NAME).arg(BATCH_ID).arg(TRAN_ID)
                .arg(VARIETY_CODE).arg(VARIETY_NAME).arg(VARIETY_TYPE).arg(WEIGHT).arg(PRICE);

        //qDebug()<<"---:"<<temp;
        TRAN_DATA.append(temp);
        TRAN_DATA.append(",");
    }
    if(batchflag<=0)
    {
        ATdb_Excute_SQL_Statements(QString("update TABLE_ATTACH set webUp='2' where businessCode='%1';").arg(businessCode));
        AttachQuery.clear();
        BusinessQuery.clear();
        return 0;
    }
    TRAN_DATA = TRAN_DATA.left(TRAN_DATA.length() - 1);
    TRAN_DATA +="]";

    AttachQuery.clear();
    BusinessQuery.clear();
    //qDebug()<<"---:"<<TRAN_DATA;

    //1
    QString BIZ_TYPE = "uploadTranData";
    //2
    QDateTime time = QDateTime::currentDateTime();                              //获取系统现在的时间
    QString REQ_TIME = time.toString("yyyy-MM-dd hh:mm:ss");
    //3
    char strRand[6] = {0x00};
    getRandStr(strRand, 6);
    QString REQ_ID = time.toString("yyyyMMddhhmmss") + strRand;
    //4
    QString AUTH_ID =  QString::fromStdString(webPublic.auth_id);
    //5
    QString PARAM = "";
    PARAM = QString("{\"TRAN_DATA\":%1}").arg(TRAN_DATA);

    QString JsonQStr = "";
    JsonQStr = QString("{\"BIZ_TYPE\":\"%1\",\"REQ_TIME\":\"%2\",\"REQ_ID\":\"%3\",\"AUTH_ID\":\"%4\",\"PARAM\":%5}")
            .arg(BIZ_TYPE).arg(REQ_TIME).arg(REQ_ID).arg(AUTH_ID).arg(PARAM);

    // 生成 SHA256 散列值
    unsigned char sha256[SHA256_DIGEST_LENGTH];
    memset(sha256, 0x00, SHA256_DIGEST_LENGTH);

    QString temp = JsonQStr + QString::fromStdString(webPublic.key);
    SHA256((unsigned char *) temp.toStdString().c_str(), temp.toStdString().length(),sha256);
    // 转换成16进制str
    char sha256Hex[SHA256_DIGEST_LENGTH*2];
    memset(sha256Hex, 0x00, SHA256_DIGEST_LENGTH*2);
    BcdToStr(sha256Hex,(char*)sha256,SHA256_DIGEST_LENGTH);

    JsonQStr.append(sha256Hex);
    //qDebug()<<"JsonQStr:"<<JsonQStr<<endl;

    string senddata = JsonQStr.toStdString();
    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 2;
    while(1)
    {
        int nPostsResult = client.Post(webPublic.WebUrl, senddata, recvdata);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            printf("[getTraceCode]____________TimeOut_____________\n");
            if (++nTimeoutCnt > 0)
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

    if(recvdata.length() <= 0)
    {
        return -6;
    }

    int start = recvdata.find_first_of('{');
    int end = recvdata.find_last_of('}');
    cout << "[start]:"<<start <<"---[end]:"<< end << endl;
    if(start == end || end <=0)
    {
        return -7;
        qDebug()<<"---------------recvdata is not JSON"<<endl;
    }

    recvdata = recvdata.substr(start, end+1);
    //cout << "recvdata:"<<recvdata << endl;

    //---------------------------------------------------------------解析
    cJSON *pJsonRoot = cJSON_Parse(recvdata.c_str());
    if (!pJsonRoot)
    {
        cJSON_Delete(pJsonRoot);
        printf("get pJsonRoot faild !\n");
        return -8;
    }

    cJSON *RESULTJson = NULL;
    RESULTJson = cJSON_GetObjectItem(pJsonRoot, "RESULT");
    if(RESULTJson == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT faild !\n");
        return -8;
    }

    cJSON *itemname = NULL;
    itemname = cJSON_GetObjectItem(RESULTJson, "RESULT_CODE");
    if(itemname == NULL)
    {
        cJSON_Delete(pJsonRoot);
        printf("get RESULT_CODE faild !\n");
        return -8;
    }
    if(strcmp(itemname->valuestring,"0000")==1) //相等返回0
    {
        cout << "2---[RESULT_CODE]:"<<itemname->valuestring<<endl;
        ATdb_Excute_SQL_Statements(QString("update TABLE_ATTACH set webUp='100' where businessCode='%1';").arg(businessCode));
        ATdb_Excute_SQL_Statements(QString("update TABLE_BUSINESS set webUp='100' where businessCode='%1' and webUp='0';").arg(businessCode));
        cJSON_Delete(pJsonRoot);
        return 8;
    }
    cout << "1---[RESULT_CODE]:"<<itemname->valuestring<<endl;
    ATdb_Excute_SQL_Statements(QString("update TABLE_ATTACH set webUp='1' where businessCode='%1';").arg(businessCode));
    ATdb_Excute_SQL_Statements(QString("update TABLE_BUSINESS set webUp='1' where businessCode='%1' and webUp='0';").arg(businessCode));

    cJSON_Delete(pJsonRoot);
    return 1;
}
