#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //for getopt, fork
#include <string.h>     //for strcat
#include <QDateTime>
#include <map>
#include <iostream>
#include <fstream>
#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "ScaleHttpService.h"
#include "../ATSocket/cJSON.h"

#define RETURN_0    "0" //0成功
#define RETURN_1    "1" //请重复调用该接口（如回收商品，回收交易）
#define RETURN_10    "-10"//数据段不是标准JSON格式
#define RETURN_11    "-11"//该接口查找电子秤数据库出错
#define RETURN_12    "-12"//客户端请求数据为空
#define RETURN_13    "-13"//服务器未找到该字段
#define RETURN_14    "-14"//服务器找到该字段，但该字段数据不对
#define RETURN_15    "-15"//JSON数组长度与传入的长度不相等
#define RETURN_16    "-16"//升级文件不存在
#define RETURN_17    "-17"//执行SQL失败

//更新电子秤服务器时间
int UpdataScaleTime(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    printf("-----------------------------------2\n");
    if(strlen(getData.c_str())<=0)
    {
        printf("getData:%s\n",getData.c_str());
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *time = cJSON_GetObjectItem(pJsonRoot, "time");
    if(time != NULL)
    {
        if(strlen(time->valuestring)!=19)
        {
            sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口time段格式错误\"}").arg(RETURN_14).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
            return -1;
        }
        char times[30] = {0};
        sprintf(times,"date -s \"%s\"",time->valuestring);
        system(times);

        memset(times,0,30);
        sprintf(times,"%s",time->valuestring);

        char systime[20] = {0};
        sprintf(systime,"%c%c%c%c%c%c%c%c%c%c%c%c",times[2],times[3],times[5],
                times[6],times[8],times[9],times[11],times[12],
                times[14],times[15],times[17],times[18]);
        ATRtcWrite(systime);
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"time\":\"%3\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip).arg(time)).toStdString();
        cJSON_Delete(pJsonRoot);
        return 1;
    }
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口缺少必填项time段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
    return 0;
}
//获取电子秤时间或检查电子秤时间
int CheckOrGetScaleTime(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    printf("-----------------------------------2\n");
    if(strlen(getData.c_str())<=0)
    {
        printf("getData:%s\n",getData.c_str());
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *ip = cJSON_GetObjectItem(pJsonRoot, "ip");
    if(ip != NULL)
    {
        printf("server-ip:%s\n",ip->valuestring);
    }
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"time\":\"%3\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip).arg(time)).toStdString();
    return 1;
}
//更新电子秤网络参数
int UpdataNetParameter(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    printf("----------------------------------------9\n");
    if(strlen(getData.c_str()) <=0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    printf("----------------------------------------9\n");
    cJSON *element = cJSON_GetObjectItem(pJsonRoot, "remoteServer");
    QString sqlStr;
    if(element != NULL)
    {
        printf("remoteServer:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=1;",element->valuestring);
        m_pInfo->m_ShareNetInfo.RemoteServer.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "internalServer");
    if(element != NULL)
    {
        printf("internalServer:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=2;",element->valuestring);
        m_pInfo->m_ShareNetInfo.InternalServer.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "Mac");
    if(element != NULL)
    {
        printf("Mac:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=3;",element->valuestring);
        m_pInfo->m_ShareNetInfo.Mac.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "IP");
    if(element != NULL)
    {
        printf("IP:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=4;",element->valuestring);
        m_pInfo->m_ShareNetInfo.Ip.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "Mask");
    if(element != NULL)
    {
        printf("Mask:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=5;",element->valuestring);
        m_pInfo->m_ShareNetInfo.Mask.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "gateway");
    if(element != NULL)
    {
        printf("Gateway:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=6;",element->valuestring);
        m_pInfo->m_ShareNetInfo.Gateway.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "netMode");
    if(element != NULL)
    {
        printf("netMode:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=7;",element->valuestring);
        m_pInfo->m_ShareNetInfo.Net_Mode.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "SSID");
    if(element != NULL)
    {
        printf("SSID:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=8;",element->valuestring);
        m_pInfo->m_ShareNetInfo.SSID.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "passWord");
    if(element != NULL)
    {
        printf("passWord:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=9;",element->valuestring);
        m_pInfo->m_ShareNetInfo.PassWord.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "Encryption");
    if(element != NULL)
    {
        printf("Encryption:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=10;",element->valuestring);
        m_pInfo->m_ShareNetInfo.Encryption.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "DHCP");
    if(element != NULL)
    {
        printf("DHCP:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=11;",element->valuestring);
        m_pInfo->m_ShareNetInfo.Dhcp.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "DNS");
    if(element != NULL)
    {
        printf("DNS:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=12;",element->valuestring);
        m_pInfo->m_ShareNetInfo.Dns.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    //m_pInfo->StartNetwork();
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
}

//获取电子秤网络信息
int GetNetParameter(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();

    if(m_pInfo->m_ShareNetInfo.RemoteServer.length()>0)
    {
        sendData.append(",\"remoteServer\":\"");
        sendData += m_pInfo->m_ShareNetInfo.RemoteServer.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.InternalServer.length()>0)
    {
        sendData += ",\"internalServer\":\"";
        sendData += m_pInfo->m_ShareNetInfo.InternalServer.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.Mac.length()>0)
    {
        sendData += ",\"Mac\":\"";
        sendData += m_pInfo->m_ShareNetInfo.Mac.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.Ip.length()>0)
    {
        sendData += ",\"IP\":\"";
        sendData += m_pInfo->m_ShareNetInfo.Ip.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.Mask.length()>0)
    {
        sendData += ",\"Mask\":\"";
        sendData += m_pInfo->m_ShareNetInfo.Mask.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.Gateway.length()>0)
    {
        sendData += ",\"gateway\":\"";
        sendData += m_pInfo->m_ShareNetInfo.Gateway.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.Net_Mode.length()>0)
    {
        sendData += ",\"netMode\":\"";
        sendData += m_pInfo->m_ShareNetInfo.Net_Mode.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.SSID.length()>0)
    {
        sendData += ",\"SSID\":\"";
        sendData += m_pInfo->m_ShareNetInfo.SSID.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.PassWord.length()>0)
    {
        sendData += ",\"passWord\":\"";
        sendData += m_pInfo->m_ShareNetInfo.PassWord.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.Encryption.length()>0)
    {
        sendData += ",\"Encryption\":\"";
        sendData += m_pInfo->m_ShareNetInfo.Encryption.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.Dhcp.length()>0)
    {
        sendData += ",\"DHCP\":\"";
        sendData += m_pInfo->m_ShareNetInfo.Dhcp.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareNetInfo.Dns.length()>0)
    {
        sendData += ",\"DNS\":\"";
        sendData += m_pInfo->m_ShareNetInfo.Dns.toStdString();
        sendData += "\"";
    }
    sendData += "}";
    return 1;
}
//更新电子秤系统信息
int UpdataScaleSystemInfo(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    char elementName[16] = {0};
    if(strlen(getData.c_str()) <=0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    QString sqlStr;
    cJSON *element = cJSON_GetObjectItem(pJsonRoot, "timeOutScreen");//必填项
    if(element != NULL)
    {
        printf("timeOutScreen:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=13;",element->valuestring);
        m_pInfo->m_ShareSystemInfo.ScreenTimeout = atoi(element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    else{
        sprintf(elementName,"%s","timeOutScreen");
        goto error;
    }
    element = cJSON_GetObjectItem(pJsonRoot, "weightPoint");//必填项
    if(element != NULL)
    {
        printf("weightPoint:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=14;",element->valuestring);
        //m_pInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT = element->valueint;
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    else{
        sprintf(elementName,"%s","weightPoint");
        goto error;
    }
    element = cJSON_GetObjectItem(pJsonRoot, "totalPoint");//必填项
    if(element != NULL)
    {
        printf("totalPoint:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=15;",element->valuestring);
        m_pInfo->m_ShareSystemInfo.Round = atoi(element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    else
    {
        sprintf(elementName,"%s","totalPoint");
        goto error;
    }
    element = cJSON_GetObjectItem(pJsonRoot, "screenBackLight");//必填项
    if(element != NULL)
    {
        printf("screenBackLight:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_SCALESETTING set value='%s' where id=16;",element->valuestring);
        m_pInfo->m_ShareSystemInfo.BackLight = atoi(element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    else
    {
        sprintf(elementName,"%s","totalRounded");
        goto error;
    }

    /*element = cJSON_GetObjectItem(pJsonRoot, "marketCode");
    if(element != NULL)
    {
        printf("marketCode:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=18;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.MarketCode.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "marketName");
    if(element != NULL)
    {
        printf("marketName:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=19;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.MarketName.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }*/
    element = cJSON_GetObjectItem(pJsonRoot, "headTail1");
    if(element != NULL)
    {
        printf("headTail1:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=20;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.HeadTail1.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "headTail2");
    if(element != NULL)
    {
        printf("headTail2:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=21;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.HeadTail2.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "headTail3");
    if(element != NULL)
    {
        printf("headTail3:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=22;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.HeadTail3.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "headTail4");
    if(element != NULL)
    {
        printf("headTail4:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=23;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.HeadTail4.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "headTail5");
    if(element != NULL)
    {
        printf("headTail5:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=24;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.HeadTail5.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "headTail6");
    if(element != NULL)
    {
        printf("headTail6:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=25;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.HeadTail6.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
    return 1;
error:
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口缺少必填项%3段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip).arg(elementName)).toStdString();
    cJSON_Delete(pJsonRoot);
    return -1;
}

//获取电子秤系统信息
int GetScaleSystemInfo(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();

    sendData += (QString(",\"timeOutScreen\":\"%1\"").arg(m_pInfo->m_ShareSystemInfo.ScreenTimeout)).toStdString();

    sendData += (QString(",\"weightPoint\":\"%1\"").arg(m_pInfo->m_ShareSystemInfo.m_curDecimal.dec_pt_WEIGHT)).toStdString();

    sendData += (QString(",\"totalPoint\":\"%1\"").arg(m_pInfo->m_ShareSystemInfo.Round)).toStdString();

    sendData += (QString(",\"screenBackLight\":\"%1\"").arg(m_pInfo->m_ShareSystemInfo.BackLight)).toStdString();

    /*if(m_pInfo->m_ShareDeviceInfo.MarketCode.length()>0)
    {
        sendData += ",\"marketCode\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.MarketCode.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.MarketName.length()>0)
    {
        sendData += ",\"marketName\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.MarketName.toStdString();
        sendData += "\"";
    }*/
    if(m_pInfo->m_ShareDeviceInfo.HeadTail1.length()>0)
    {
        sendData += ",\"headTail1\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.HeadTail1.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.HeadTail2.length()>0)
    {
        sendData += ",\"headTail2\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.HeadTail2.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.HeadTail3.length()>0)
    {
        sendData += ",\"headTail3\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.HeadTail3.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.HeadTail4.length()>0)
    {
        sendData += ",\"headTail4\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.HeadTail4.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.HeadTail5.length()>0)
    {
        sendData += ",\"headTail5\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.HeadTail5.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.HeadTail6.length()>0)
    {
        sendData += ",\"headTail6\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.HeadTail6.toStdString();
        sendData += "\"";
    }
    sendData += "}";
    return 1;
}
//更新电子秤用户信息
int UpdataScaleUserInfo(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    if(strlen(getData.c_str()) <=0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    QString sqlStr;
    cJSON *element = cJSON_GetObjectItem(pJsonRoot, "marketCode");
    if(element != NULL)
    {
        printf("marketCode:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=18;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.MarketCode.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "marketName");
    if(element != NULL)
    {
        printf("marketName:%s\n",element->valuestring);
        sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=19;",element->valuestring);
        m_pInfo->m_ShareDeviceInfo.MarketName.sprintf("%s",element->valuestring);
        ATdb_Excute_SQL_Statements(sqlStr);
    }
    element = cJSON_GetObjectItem(pJsonRoot, "deviceId");
    if(element != NULL)
    {
        printf("deviceId:%s\n",element->valuestring);
        if(strlen(element->valuestring)>0)
        {
            sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=12;",element->valuestring);
            m_pInfo->m_ShareDeviceInfo.DeviceId.sprintf("%s",element->valuestring);
            ATdb_Excute_SQL_Statements(sqlStr);
        }
    }
    element = cJSON_GetObjectItem(pJsonRoot, "deviceType");
    if(element != NULL)
    {
        printf("deviceType:%s\n",element->valuestring);
        if(strlen(element->valuestring)>0)
        {
            sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=13;",element->valuestring);
            m_pInfo->m_ShareDeviceInfo.DeviceType.sprintf("%s",element->valuestring);
            ATdb_Excute_SQL_Statements(sqlStr);
        }
    }
    element = cJSON_GetObjectItem(pJsonRoot, "boothId");
    if(element != NULL)
    {
        printf("boothId:%s\n",element->valuestring);
        if(strlen(element->valuestring)>0)
        {
            sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=14;",element->valuestring);
            m_pInfo->m_ShareDeviceInfo.BoothId.sprintf("%s",element->valuestring);
            ATdb_Excute_SQL_Statements(sqlStr);
        }
    }
    element = cJSON_GetObjectItem(pJsonRoot, "userName");
    if(element != NULL)
    {
        printf("userName:%s\n",element->valuestring);
        if(strlen(element->valuestring)>0)
        {
            sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=15;",element->valuestring);
            m_pInfo->m_ShareDeviceInfo.BoothName.sprintf("%s",element->valuestring);
            ATdb_Excute_SQL_Statements(sqlStr);
        }
    }
    element = cJSON_GetObjectItem(pJsonRoot, "userCode");
    if(element != NULL)
    {
        printf("userCode:%s\n",element->valuestring);
        if(strlen(element->valuestring)>0)
        {
            sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=16;",element->valuestring);
            m_pInfo->m_ShareDeviceInfo.BoothCode.sprintf("%s",element->valuestring);
            ATdb_Excute_SQL_Statements(sqlStr);
        }
    }
    element = cJSON_GetObjectItem(pJsonRoot, "payId");
    if(element != NULL)
    {
        printf("payId:%s\n",element->valuestring);
        if(strlen(element->valuestring)>0)
        {
            sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=1;",element->valuestring);
            m_pInfo->m_ShareCustomerInfo.AlipayId.sprintf("%s",element->valuestring);
            ATdb_Excute_SQL_Statements(sqlStr);
        }
    }
    element = cJSON_GetObjectItem(pJsonRoot, "payNum");
    if(element != NULL)
    {
        printf("payNum:%s\n",element->valuestring);
        if(strlen(element->valuestring)>0)
        {
            sqlStr.sprintf("update TABLE_USERSETTING set value='%s' where id=2;",element->valuestring);
            m_pInfo->m_ShareCustomerInfo.AlipayNo.sprintf("%s",element->valuestring);
            ATdb_Excute_SQL_Statements(sqlStr);
        }
    }

    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
    return 1;
}
//获取电子秤用户信息
int GetScaleUserInfo(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();

    if(m_pInfo->m_ShareDeviceInfo.MarketCode.length()>0)
    {
        sendData += ",\"marketCode\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.MarketCode.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.MarketName.length()>0)
    {
        sendData += ",\"marketName\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.MarketName.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.DeviceId.length()>0)
    {
        sendData += ",\"deviceId\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.DeviceId.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.DeviceType.length()>0)
    {
        sendData += ",\"deviceType\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.DeviceType.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.BoothId.length()>0)
    {
        sendData += ",\"boothId\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.BoothId.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.BoothName.length()>0)
    {
        sendData += ",\"userName\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.BoothName.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareDeviceInfo.BoothCode.length()>0)
    {
        sendData += ",\"userCode\":\"";
        sendData += m_pInfo->m_ShareDeviceInfo.BoothCode.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareCustomerInfo.AlipayId.length()>0)
    {
        sendData += ",\"payId\":\"";
        sendData += m_pInfo->m_ShareCustomerInfo.AlipayId.toStdString();
        sendData += "\"";
    }
    if(m_pInfo->m_ShareCustomerInfo.AlipayNo.length()>0)
    {
        sendData += ",\"payNum\":\"";
        sendData += m_pInfo->m_ShareCustomerInfo.AlipayNo.toStdString();
        sendData += "\"";
    }

    sendData += "}";

    return 1;
}
//更新电子秤登录用户信息
int UpdataScaleLoginUser(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    char elementName[16] = {0};
    int userListSize = 0;
    if(strlen(getData.c_str())<=0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
//    cJSON *element = cJSON_GetObjectItem(pJsonRoot, "loginUserSize");
//    if(element != NULL)
//    {
//        printf("loginUserSize:%d\n",element->valueint);
//        userListSize = element->valueint;
//    }
//    else
//    {
//        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口缺少必填项loginUserSize段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
//        cJSON_Delete(pJsonRoot);
//        return -1;
//    }
    cJSON *userListArry = cJSON_GetObjectItem(pJsonRoot, "userList");
    if(NULL == userListArry)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口缺少必填项userList段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    if((userListSize = cJSON_GetArraySize(userListArry)) <= 0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"异常：列表参数为空\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    struct_tb_seller_info *seller = new struct_tb_seller_info[userListSize];
    cJSON *itemInfo = NULL;
    char *p = NULL;
    cJSON *item_d = NULL;
    cJSON *json_d = NULL;

    for(int i=0; i<userListSize; i++)
    {
        item_d = cJSON_GetArrayItem(userListArry, i);
        p = cJSON_PrintUnformatted(item_d);
        json_d = cJSON_Parse(p);
        itemInfo = cJSON_GetObjectItem(json_d, "userName");
        if(itemInfo != NULL)	//必填
        {
            printf("userName:%s\n",itemInfo->valuestring);
            if(strlen(itemInfo->valuestring) <= 0)
            {
                sprintf(elementName,"%s","userName");
                goto error;
            }
            seller[i].name.sprintf("%s" ,itemInfo->valuestring);
        }
        else{
            sprintf(elementName,"%s","userName");
            goto error;
        }
        itemInfo = cJSON_GetObjectItem(json_d, "passWord");
        if(itemInfo != NULL)	//必填
        {
            printf("passWord:%s\n",itemInfo->valuestring);
            if(strlen(itemInfo->valuestring) <= 0)
            {
                sprintf(elementName,"%s","passWord");
                goto error;
            }
            seller[i].pass.sprintf("%s" ,itemInfo->valuestring);
        }
        else{
            sprintf(elementName,"%s","passWord");
            goto error;
        }
    }
    ATdb_Excute_SQL_Statements("delete from TABLE_SELLERSINFO;");
    ATdb_Insert_DataList_By_TableName_DataList(TABLE_SELLERSINFO, seller, userListSize);
    delete[] seller;
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
    return 1;
error:
    delete[] seller;
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口缺少必填项%3段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip).arg(elementName)).toStdString();
    cJSON_Delete(pJsonRoot);
    return -1;
}

//获取电子秤用户登录信息
int GetScaleLoginUser(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    QSqlQuery query;
    int dbSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from TABLE_SELLERSINFO;",&query);
    if (dbSize <= 0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"userList\":[],\"message\":\"成功：电子秤数据库无数据\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return 0;
    }
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\",\"userList\":[").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();

    for(int i=0; i<dbSize; i++)
    {
        query.next();
        sendData = "{";
        if(query.record().value(TABLE_SELLERSINFO_NAME).toString().length()>0)
        {
            sendData += ",\"userName\":\"";
            sendData += query.record().value(TABLE_SELLERSINFO_NAME).toString().toStdString();
            sendData += "\"";
        }
        if(query.record().value(TABLE_SELLERSINFO_PASS).toString().length()>0)
        {
            sendData += ",\"passWord\":\"";
            sendData += query.record().value(TABLE_SELLERSINFO_PASS).toString().toStdString();
            sendData += "\"";
        }
        if(query.record().value(TABLE_SELLERSINFO_SELLERCARD).toString().length()>0)
        {
            sendData += ",\"cardNum\":\"";
            sendData += query.record().value(TABLE_SELLERSINFO_SELLERCARD).toString().toStdString();
            sendData += "\"";
        }
        if(query.record().value(TABLE_SELLERSINFO_PROPRIETORCODE).toString().length()>0)
        {
            sendData += ",\"proprietorCode\":\"";
            sendData += query.record().value(TABLE_SELLERSINFO_PROPRIETORCODE).toString().toStdString();
            sendData += "\"";
        }
        if(i<dbSize-1)
            sendData += "},";
        else
            sendData += "}";
    }
    sendData += "]}";
    query.clear();
    return 1;
}

//更新电子秤用户开关
int UpdataScaleUserSwitch(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    if(0 == getData.compare("{}"))
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *element = cJSON_GetObjectItem(pJsonRoot, "userSwitch");
    QString userSwitch;
    if(element != NULL)
    {
        printf("userSwitch:%s\n",element->valuestring);
        userSwitch.sprintf("%s",element->valuestring);
    }
    else
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口未找到userSwitch段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    if(userSwitch.length() != 7)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口userSwitch段数据错误\"}").arg(RETURN_14).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    bool ok;
    m_pInfo->m_ShareUserSwitchInfo.TotalPriceSwitch = userSwitch.mid(0,1).toInt(&ok);
    m_pInfo->m_ShareUserSwitchInfo.SavePluSwitch = userSwitch.mid(1,1).toInt(&ok);
    m_pInfo->m_ShareUserSwitchInfo.SavePriceSwitch = userSwitch.mid(2,1).toInt(&ok);
    m_pInfo->m_ShareUserSwitchInfo.RecordSwitch = userSwitch.mid(3,1).toInt(&ok);
    m_pInfo->m_ShareUserSwitchInfo.PrintSwitch = userSwitch.mid(4,1).toInt(&ok);
    m_pInfo->m_ShareUserSwitchInfo.fThresholdVale = userSwitch.mid(5,2).toInt(&ok)/100.0;
    ATdb_Excute_SQL_Statements(QString("update TABLE_SCALESETTING set value='%1' where upper='3';").arg(userSwitch));
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
    return 1;
}
//获取电子秤用户开关
int GetScaleUserSwitch(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    QString userSwitch;
    ATdb_Select_ReturnQString_By_SQL_Statements("select value from TABLE_SCALESETTING where upper='3';",&userSwitch);

    if(userSwitch.length() != 7)
    {
        //数据库数据出错
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：电子秤数据库数据错误\"}").arg(RETURN_11).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"userSwitch\":\"%3\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip).arg(userSwitch)).toStdString();
    return 1;
}
//更新电子秤系统开关
int UpdataScaleSystemSwitch(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    if(0 == getData.compare("{}"))
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *element = cJSON_GetObjectItem(pJsonRoot, "systemSwitch");
    QString systemSwitch;
    if(element != NULL)
    {
        printf("systemSwitch:%s\n",element->valuestring);
        systemSwitch.sprintf("%s",element->valuestring);
    }
    else{
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口未找到systemSwitch段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    if(systemSwitch.length() != 15)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口systemSwitch段数据错误\"}").arg(RETURN_14).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    bool ok;
    m_pInfo->m_ShareSysSwitchInfo.LoginMode = systemSwitch.mid(0,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.PrintPaper = systemSwitch.mid(1,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.CashSwitch = systemSwitch.mid(2,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.CameraSwitch = systemSwitch.mid(3,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.RfidSwitch = systemSwitch.mid(4,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.BluetoothSwitch = systemSwitch.mid(5,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.ZigbeeSwitch = systemSwitch.mid(6,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.PosSwitch = systemSwitch.mid(7,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.ScannerSwitch = systemSwitch.mid(8,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.ManageSwitch = systemSwitch.mid(9,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.WebSwitch = systemSwitch.mid(10,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.AlipaySwitch = systemSwitch.mid(11,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.WechatSwitch = systemSwitch.mid(12,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.OtherSwitch = systemSwitch.mid(13,1).toInt(&ok);
    m_pInfo->m_ShareSysSwitchInfo.DebugSwitch = systemSwitch.mid(14,1).toInt(&ok);
    ATdb_Excute_SQL_Statements(QString("update TABLE_SCALESETTING set value='%1' where upper='4';").arg(systemSwitch));
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    return 1;
}

//获取电子秤系统开关
int GetScaleSystemSwitch(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    QString systemSwitch;
    ATdb_Select_ReturnQString_By_SQL_Statements("select value from TABLE_SCALESETTING where upper='4';",&systemSwitch);

    if(systemSwitch.length() != 15)
    {
        //数据库数据出错
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：电子秤数据库数据错误\"}").arg(RETURN_11).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"systemSwitch\":\"%3\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip).arg(systemSwitch)).toStdString();
    return 1;
}
//更新电子秤商品列表
int UpdataPLUList(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    int pluListSize = 0;
    if(0 == getData.compare("{}"))
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pluListArry = cJSON_GetObjectItem(pJsonRoot, "pluList");
    if(NULL == pluListArry)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口未找到pluList段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    if((pluListSize = cJSON_GetArraySize(pluListArry)) <=0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"异常：列表参数为空\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    struct_tb_product *plu= new struct_tb_product[pluListSize];
    cJSON *itemInfo = NULL;
    char *p = NULL;
    cJSON *item_d = NULL;
    cJSON *json_d = NULL;
    char elementName[16] = {0};
    for(int i = 0; i<pluListSize;i++)
    {
        item_d = cJSON_GetArrayItem(pluListArry, i);
        p = cJSON_PrintUnformatted(item_d);
        json_d = cJSON_Parse(p);
        itemInfo = cJSON_GetObjectItem(json_d, "goodsCode");
        if(itemInfo != NULL)	//必填项
        {
            //printf("goodsCode:%s\n",itemInfo->valuestring);
            if(strlen(itemInfo->valuestring) <= 0)
            {
                sprintf(elementName,"%s","goodsCode");
                goto error;
            }
            plu[i].goodsCode.sprintf("%s" ,itemInfo->valuestring);
        }
        else
        {
            sprintf(elementName,"%s","goodsCode");
            goto error;
        }
        itemInfo = cJSON_GetObjectItem(json_d, "goodsName");
        if(itemInfo != NULL)	//必填项
        {
            printf("goodsName:%s\n",itemInfo->valuestring);
            if(strlen(itemInfo->valuestring) <= 0)
            {
                sprintf(elementName,"%s","goodsName");
                goto error;
            }
            plu[i].goodsName.sprintf("%s" ,itemInfo->valuestring);
        }
        else
        {
            sprintf(elementName,"%s","goodsName");
            goto error;
        }
        itemInfo = cJSON_GetObjectItem(json_d, "goodsOrigin");
        if(itemInfo != NULL)
        {
            //printf("goodsOrigin:%s\n",itemInfo->valuestring);
            plu[i].goodsOrigin.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "pluCode");
        if(itemInfo != NULL)
        {
            //printf("pluCode:%s\n",itemInfo->valuestring);
            plu[i].pluCode.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "goodsKey");
        if(itemInfo != NULL)
        {
            //printf("goodsKey:%s\n",itemInfo->valuestring);
            plu[i].goodsKey.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "goodsType");
        if(itemInfo != NULL)
        {
            //printf("goodsType:%s\n",itemInfo->valuestring);
            plu[i].groupNo.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "batchCode");
        if(itemInfo != NULL)
        {
            //printf("batchCode:%s\n",itemInfo->valuestring);
            plu[i].batchCode.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "traceCode");
        if(itemInfo != NULL)
        {
            //printf("traceCode:%s\n",itemInfo->valuestring);
            plu[i].batchCode.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "goodsPrice");
        if(itemInfo != NULL)
        {
            //printf("goodsPrice:%d\n",itemInfo->valueint);
            plu[i].lastPrice.sprintf("%d" ,itemInfo->valueint);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "goodsWeight");
        if(itemInfo != NULL)
        {
            //printf("goodsWeight:%s\n",itemInfo->valuestring);
            plu[i].goodsWeight.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "salesMode");
        if(itemInfo != NULL)
        {
            //printf("salesMode:%s\n",itemInfo->valuestring);
            plu[i].isPcs.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "isLimit");
        if(itemInfo != NULL)
        {
            //printf("isLimit:%s\n",itemInfo->valuestring);
            plu[i].isLimit.sprintf("%s" ,itemInfo->valuestring);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "maxPrice");
        if(itemInfo != NULL)
        {
            //printf("maxPrice:%d\n",itemInfo->valueint);
            plu[i].maxPrice.sprintf("%d" ,itemInfo->valueint);
        }
        itemInfo = cJSON_GetObjectItem(json_d, "minPrice");
        if(itemInfo != NULL)
        {
            //printf("minPrice:%d\n",itemInfo->valueint);
            plu[i].minPrice.sprintf("%d" ,itemInfo->valueint);
        }
    }
    ATdb_Excute_SQL_Statements("delete from TABLE_PRODUCT;");
    ATdb_Insert_DataList_By_TableName_DataList(TABLE_PRODUCT, plu, pluListSize);
    delete[] plu;
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
    return 1;
error:
    delete[] plu;
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口缺少必填项%3段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip).arg(elementName)).toStdString();
    cJSON_Delete(pJsonRoot);
    return -1;
}

//获取电子秤商品列表
int GetScalePluList(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    static unsigned int 	m_nPluListCount = 0 ;

    QSqlQuery query;
    int productSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(QString("select * from TABLE_PRODUCT where id > %1 limit 50;").arg(m_nPluListCount),&query);
    if (productSize <= 0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"pluList\":[],\"message\":\"成功：数据库无数据\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        query.clear();
        return -1;
    }
    if(productSize == 50)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\",\"pluList\":[").arg(RETURN_1).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    }
    else
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\",\"pluList\":[").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    }

    for(int i=0; i<productSize; i++)
    {
        query.next();
        sendData += "{\"goodsCode\":\"";
        sendData += query.record().value(TABLE_PRODUCT_GOODSCODE).toString().toStdString();
        sendData += "\"";

        sendData += ",\"goodsName\":\"";
        sendData += query.record().value(TABLE_PRODUCT_GOODSNAME).toString().toStdString();
        sendData += "\"";

        if(query.record().value(TABLE_PRODUCT_GOODSORIGIN).toString().length()>0)
        {
            sendData += ",\"goodsOrigin\":\"";
            sendData += query.record().value(TABLE_PRODUCT_GOODSORIGIN).toString().toStdString();
            sendData += "\"";
        }
        if(query.record().value(TABLE_PRODUCT_GROUPNO).toString().length()>0)
        {
            sendData += ",\"goodsType\":\"";
            sendData += query.record().value(TABLE_PRODUCT_GROUPNO).toString().toStdString();
            sendData += "\"";
        }
        if(query.record().value(TABLE_PRODUCT_BATCHCODE).toString().length()>0)
        {
            sendData += ",\"batchCode\":\"";
            sendData += query.record().value(TABLE_PRODUCT_BATCHCODE).toString().toStdString();
            sendData += "\"";
        }
        if(query.record().value(TABLE_PRODUCT_TRACECODE).toString().length()>0)
        {
            sendData += ",\"traceCode\":\"";
            sendData += query.record().value(TABLE_PRODUCT_TRACECODE).toString().toStdString();;
            sendData += "\"";
        }
        if(query.record().value(TABLE_PRODUCT_LASTPRICE).toString().length()>0)
        {
            sendData += ",\"goodsPrice\":";//int
            sendData += query.record().value(TABLE_PRODUCT_LASTPRICE).toString().toStdString();
        }
        if(query.record().value(TABLE_PRODUCT_GOODSWEIHT).toString().length()>0)
        {
            sendData += ",\"goodsWeight\":";
            sendData += query.record().value(TABLE_PRODUCT_GOODSWEIHT).toString().toStdString();
            sendData += "\"";
        }
        if(query.record().value(TABLE_PRODUCT_ISPCS).toString().length()>0)
        {
            sendData += ",\"salesMode\":";//int
            sendData += query.record().value(TABLE_PRODUCT_ISPCS).toString().toStdString();
        }
        if(query.record().value(TABLE_PRODUCT_ISLIMIT).toString().length()>0)
        {
            sendData += ",\"isPrice\":";//int
            sendData += query.record().value(TABLE_PRODUCT_ISLIMIT).toString().toStdString();
        }
        if(query.record().value(TABLE_PRODUCT_MAXPRICE).toString().length()>0)
        {
            sendData += ",\"maxPrice\":";
            sendData += query.record().value(TABLE_PRODUCT_MAXPRICE).toString().toStdString();
        }
        if(query.record().value(TABLE_PRODUCT_MINPRICE).toString().length()>0)
        {
            sendData += ",\"minPrice\":";
            sendData += query.record().value(TABLE_PRODUCT_MINPRICE).toString().toStdString();
        }
        if(i<productSize-1)
                sendData += "},";
        else
                sendData += "}";
    }
    sendData += "]}";
    m_nPluListCount += productSize;
    if(productSize != 70)
            m_nPluListCount = 0;

    query.clear();
    return 1;
}
//更新电子秤商品快捷键
int UpdataGoodsHotKey(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    int keyListSize = 0;
    if(strlen(getData.c_str()) <= 0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
//    cJSON *element = cJSON_GetObjectItem(pJsonRoot, "keyListSize");
//    if(element != NULL)
//    {
//        printf("keyListSize:%d\n",element->valueint);
//        keyListSize = element->valueint;
//    }
//    else
//    {
//        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口未找到keyListSize段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
//        cJSON_Delete(pJsonRoot);
//        return -1;
//    }
    cJSON *keyListArry = cJSON_GetObjectItem(pJsonRoot, "hotKeyList");
    if(NULL == keyListArry)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口未找到hotKeyList段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    if((keyListSize = cJSON_GetArraySize(keyListArry)) <= 0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"异常：参数列表为空\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }

    struct_tb_convert *key= new struct_tb_convert[keyListSize];
    cJSON *itemInfo = NULL;
    char *p = NULL;
    cJSON *item_d = NULL;
    cJSON *json_d = NULL;
    char elementName[16] = {0};
    for(int i = 0; i<keyListSize;i++)
    {
        key[i].ClearAllData();
        item_d = cJSON_GetArrayItem(keyListArry, i);
        p = cJSON_PrintUnformatted(item_d);
        json_d = cJSON_Parse(p);
        itemInfo = cJSON_GetObjectItem(json_d, "hotKey");
        if(itemInfo != NULL)	//必填项
        {
            printf("hotKey:%s\n",itemInfo->valuestring);
            if(strlen(itemInfo->valuestring)<=0)
            {
                sprintf(elementName,"%s","hotKey");
                goto error;
            }
            key[i].hotKey.sprintf("%s" ,itemInfo->valuestring);
        }
        else
        {
            sprintf(elementName,"%s","hotKey");
            goto error;
        }
        itemInfo = cJSON_GetObjectItem(json_d, "goodsCode");
        if(itemInfo != NULL)	//必填项
        {
            printf("goodsCode:%s\n",itemInfo->valuestring);
            if(strlen(itemInfo->valuestring)<=0)
            {
                sprintf(elementName,"%s","goodsCode");
                goto error;
            }
            key[i].goodsCode.sprintf("%s" ,itemInfo->valuestring);
        }
        else
        {
            sprintf(elementName,"%s","goodsCode ");
            goto error;
        }

    }  
    ATdb_Excute_SQL_Statements("delete from TABLE_CONVERT;");
    ATdb_Insert_DataList_By_TableName_DataList(TABLE_CONVERT, key, keyListSize);
    delete[] key;
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
    return 1;
error:
    delete[] key;
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口缺少必填项%3段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip).arg(elementName)).toStdString();
    cJSON_Delete(pJsonRoot);
    return -1;
}

//获取电子秤商品快捷键
int GetGoodsHotKey(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    QSqlQuery query;
    int convertSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from TABLE_CONVERT ;",&query);
    if (convertSize <= 0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"hotKeyList\":[],\"message\":\"成功：数据库无数据\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        query.clear();
        return -1;
    }
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\",\"hotKeyList\":[").arg(RETURN_1).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();

    for(int i=0; i<convertSize; i++)
    {
        query.next();
        sendData += "{\"goodsCode\":\"";
        sendData += query.record().value(TABLE_CONVERT_GOODSCODE).toString().toStdString();
        sendData += "\"";

        sendData += ",\"hotKey\":\"";
        sendData += query.record().value(TABLE_CONVERT_HOTKEY).toString().toStdString();
        sendData += "\"";
        if(i<convertSize-1)
                sendData += "},";
        else
                sendData += "}";
    }
    sendData += "]}";
    query.clear();
    return 1;
}

//获取电子秤交易信息
int GetScaleSaleInfo(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    QSqlQuery AttachQuery;
    int AttachSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select * from TABLE_ATTACH where  windUp=0 limit 10;",&AttachQuery);
    if (AttachSize <= 0)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"saleList\":[],\"message\":\"：数据库小计表无数据\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        AttachQuery.clear();
        return 0;
    }
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\",\"saleList\":[").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    QSqlQuery BusinessQuery;
    for(int i=0 ;i < AttachSize; i++)
    {
        AttachQuery.next();
        BusinessQuery.clear();
        QString businessCode = AttachQuery.record().value(TABLE_ATTACH_BUSINESSCODE).toString();
        int BusinessSize = ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements(QString("select * from TABLE_BUSINESS where  businessCode=%1 ;").arg(businessCode),&BusinessQuery);
        if (BusinessSize <= 0)
        {
            ATdb_Excute_SQL_Statements(QString("delete from TABLE_ATTACH where businessCode=%1 ;").arg(businessCode));
            continue;
        }

        sendData += "{\"serialCode\":\"";
        sendData += AttachQuery.record().value(TABLE_ATTACH_SERIAL).toString().toStdString();
        sendData += "\",\"saleNum\":";//int
        sendData += AttachQuery.record().value(TABLE_ATTACH_SALENUM).toString().toStdString();

        if(AttachQuery.record().value(TABLE_ATTACH_SUBTOTAL).toString().length() >0 )
        {
            sendData += ",\"subTotal\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_SUBTOTAL).toString().toStdString();
            sendData += "\"";
        }
        if(AttachQuery.record().value(TABLE_ATTACH_PAYMONEY).toString().length() >0 )
        {
            sendData += ",\"payMoney\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_PAYMONEY).toString().toStdString();
            sendData += "\"";
        }
        if(AttachQuery.record().value(TABLE_ATTACH_PAYTYPE).toString().length() >0 )
        {
            sendData += ",\"payType\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_PAYTYPE).toString().toStdString();
            sendData += "\"";
        }
        if(AttachQuery.record().value(TABLE_ATTACH_CARDNUM).toString().length() >0 )
        {
            sendData += ",\"cardNum\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_CARDNUM).toString().toStdString();
            sendData += "\"";
        }
        if(AttachQuery.record().value(TABLE_ATTACH_SCALETIME).toString().length() >0 )
        {
            sendData += ",\"saleTime\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_SCALETIME).toString().toStdString();
            sendData += "\"";
        }
        if(AttachQuery.record().value(TABLE_ATTACH_SELLERINFO).toString().length() >0 )
        {
            sendData += ",\"sellerInfo\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_SELLERINFO).toString().toStdString();
            sendData += "\"";
        }
        if(AttachQuery.record().value(TABLE_ATTACH_BUYERINFO).toString().length() >0 )
        {
            sendData += ",\"buyerInfo\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_BUYERINFO).toString().toStdString();
            sendData += "\"";
        }
        if(AttachQuery.record().value(TABLE_ATTACH_ISPRINT).toString().length() >0 )
        {
            sendData += ",\"isPrint\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_ISPRINT).toString().toStdString();
            sendData += "\"";
        }
        if(AttachQuery.record().value(TABLE_ATTACH_WEBUP).toString().length() >0 )
        {
            sendData += ",\"webUp\":\"";
            sendData += AttachQuery.record().value(TABLE_ATTACH_WEBUP).toString().toStdString();
            sendData += "\"";
        }
        sendData += ",\"detailList\":[";
        for(int j=0 ; j<BusinessSize ; j++)
        {
            BusinessQuery.next();
            sendData += "{\"goodsCode\":\"";
            sendData += BusinessQuery.record().value(TABLE_BUSINESS_GOODSCODE).toString().toStdString();
            sendData += "\",\"goodsName\":\"";//int
            sendData += BusinessQuery.record().value(TABLE_BUSINESS_GOODSNAME).toString().toStdString();
            sendData += "\"";

            if(BusinessQuery.record().value(TABLE_BUSINESS_BATCHCODE).toString().length() >0 )
            {
                sendData += ",\"batchCode\":\"";
                sendData += BusinessQuery.record().value(TABLE_BUSINESS_BATCHCODE).toString().toStdString();
                sendData += "\"";
            }
            if(BusinessQuery.record().value(TABLE_BUSINESS_TRACECODE).toString().length() >0 )
            {
                sendData += ",\"traceCode\":\"";
                sendData += BusinessQuery.record().value(TABLE_BUSINESS_TRACECODE).toString().toStdString();
                sendData += "\"";
            }
            if(BusinessQuery.record().value(TABLE_BUSINESS_GOODSORIGIN).toString().length() >0 )
            {
                sendData += ",\"goodsOrigin\":\"";
                sendData += BusinessQuery.record().value(TABLE_BUSINESS_GOODSORIGIN).toString().toStdString();
                sendData += "\"";
            }
            if(0 == BusinessQuery.record().value(TABLE_BUSINESS_SALEMODE).toString().compare("1"))
            {
                if(BusinessQuery.record().value(TABLE_BUSINESS_PIECES).toString().length() >0 )
                {
                    sendData += ",\"goodsWeight\":\"";
                    sendData += BusinessQuery.record().value(TABLE_BUSINESS_PIECES).toString().toStdString();
                    sendData += "\"";
                }
            }
            else
            {
                if(BusinessQuery.record().value(TABLE_BUSINESS_WEIGHT).toString().length() >0 )
                {
                    sendData += ",\"goodsWeight\":\"";
                    sendData += BusinessQuery.record().value(TABLE_BUSINESS_WEIGHT).toString().toStdString();
                    sendData += "\"";
                }
            }
            if(BusinessQuery.record().value(TABLE_BUSINESS_PRICE).toString().length() >0 )
            {
                sendData += ",\"goodsPrice\":\"";
                sendData += BusinessQuery.record().value(TABLE_BUSINESS_PRICE).toString().toStdString();
                sendData += "\"";
            }
            if(BusinessQuery.record().value(TABLE_BUSINESS_TOTAL).toString().length() >0 )
            {
                sendData += ",\"goodsTotal\":\"";
                sendData += BusinessQuery.record().value(TABLE_BUSINESS_TOTAL).toString().toStdString();
                sendData += "\"";
            }
            if(BusinessQuery.record().value(TABLE_BUSINESS_ISOVERSALE).toString().length() >0 )
            {
                sendData += ",\"isOverSale\":\"";
                sendData += BusinessQuery.record().value(TABLE_BUSINESS_ISOVERSALE).toString().toStdString();
                sendData += "\"";
            }
            if(BusinessQuery.record().value(TABLE_BUSINESS_SALEMODE).toString().length() >0 )
            {
                sendData += ",\"saleMode\":\"";
                sendData += BusinessQuery.record().value(TABLE_BUSINESS_SALEMODE).toString().toStdString();
                sendData += "\"";
            }
            if(BusinessQuery.record().value(TABLE_BUSINESS_TIME).toString().length() >0 )
            {
                sendData += ",\"saleTime\":\"";
                sendData += BusinessQuery.record().value(TABLE_BUSINESS_TIME).toString().toStdString();
                sendData += "\"";
            }
            /*QString picture = BusinessQuery.record().value(TABLE_BUSINESS_PICTURE).toString();
            if(picture.length() >0 )
            {
                sendData += ",\"pictureName\":\"";
                sendData += picture.toStdString();
                sendData += "\"";

                char strFilePath[128] = {0};
                printf("pictureName:%s\n", picture.toStdString().c_str());
                sprintf(strFilePath, "/root/Application/photo/%s.jpg", picture.toStdString().c_str());
                FILE *fp = fopen(strFilePath,"rb");
                if (fp)
                {
                    long curpos = ftell(fp);
                    fseek(fp,0L,SEEK_END);
                    long size = ftell(fp);
                    fseek(fp, curpos, SEEK_SET);
                    char *strFile = new char[size + 1]; // 开空间，注意 + 1
                    fread(strFile, size, 1, fp);
                    printf("size:%ld\n", size);
                    //printf("strFile:%s\n", strFile);
                    char *base64 = new char[size*2+1];
                    long len = EVP_EncodeBlock((unsigned char*)base64, (unsigned char *)strFile, size);
                    printf("len:%ld\n", len);
                    //printf("base64:%s\n", base64);
                    fclose(fp);

                    sendData += ",\"pictureData\":\"";
                    sendData += base64;
                    sendData += "\"";
                    //上传成功删除对应的图片
                    char delPicture[128] = {0};
                    sprintf(delPicture,"rm -rf %s" ,strFilePath);
                    system(delPicture);

                    delete strFile;
                    delete base64;
                }
                else
                {
                    sendData += ",\"pictureData\":\"";
                    sendData += "";
                    sendData += "\"";
                }
            }*/
            if(BusinessSize-1 == j)
            {
                sendData += "}";
            }
            else
            {
                sendData += "},";
            }
        }
        sendData += "]";
        if(AttachSize-1 == i)
        {
            sendData += "}";
        }
        else
        {
            sendData += "},";
        }
        ATdb_Excute_SQL_Statements(QString("update TABLE_ATTACH set windUp='1' where businessCode='%1';").arg(businessCode));
    }
    sendData += "]}";
    AttachQuery.clear();
    BusinessQuery.clear();
    return 1;
}
//电子秤软件升级
int UpdataScaleProgram(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    m_pInfo->ShowMessage("程序升级中，请等待电子秤重启...");
    m_pInfo->UpdateGui();
    QString programName = "/root/Application/update.tar.bz2";
    QFile DBfile(programName);
    if(DBfile.exists())
    {
        //如果文件存在，就解压文件执行脚本完成升级
        if (system("tar -xjvf /root/Application/update.tar.bz2") == 0)
        {
            printf("__________tar -xjvf update.tar.bz2 success___________\n");
            if (system("/root/Application/update/update.sh") != 0)
            {
                printf("__________update.sh fail___________\n");
                sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"update.sh执行失败\"}").arg(RETURN_16).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
            }
        }
        else{
            sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"解压升级包文件失败\"}").arg(RETURN_16).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
            printf("解压升级包文件失败\n");
        }
    }
    else
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"升级文件不存在\"}").arg(RETURN_16).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    }
    return 0;
}
//电子秤执行SQL
int PerformSQL(CShareInfo *m_pInfo,string & sendData,string & getData)
{
    if(0 == getData.compare("{}"))
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口无入参\"}").arg(RETURN_12).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *pJsonRoot = cJSON_Parse(getData.c_str());
    if(!pJsonRoot)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口入参不是JSON格式\"}").arg(RETURN_10).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        return -1;
    }
    cJSON *element = cJSON_GetObjectItem(pJsonRoot, "SQL");
    if(element == NULL)
    {
        sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：该接口未找到SQL段\"}").arg(RETURN_13).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
        cJSON_Delete(pJsonRoot);
        return -1;
    }
    if(strlen(element->valuestring)>0)
    {
        QString sql;
        sql.sprintf("%s",element->valuestring);
        QStringList strlist = sql.split(";");
        printf("strlist.size:%d\n",strlist.size());
        for(int i=0;i<strlist.size()-1;i++)
        {
            if(ATdb_Excute_SQL_Statements(strlist.at(i)) != 1)
            {
                sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"失败：执行%3条SQL错误，后面不执行\"}").arg(RETURN_17).arg(m_pInfo->m_ShareNetInfo.Ip).arg(i)).toStdString();
                cJSON_Delete(pJsonRoot);
                return -1;
            }
        }  
    }
    sendData = (QString("{\"results\":%1,\"IP\":\"%2\",\"message\":\"成功\"}").arg(RETURN_0).arg(m_pInfo->m_ShareNetInfo.Ip)).toStdString();
    cJSON_Delete(pJsonRoot);
    return 1;
}



























