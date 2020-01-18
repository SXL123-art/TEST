#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>

#include <map>
#include <iostream>

#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <curl/curl.h>

#include "icbc.h"

#include "../../../ATLib/SysApi.h"
#include "../../../ATSocket/cJSON.h"
#include "../../../ATSocket/CHttpClient.h"
#include "../../../ATLib/ATLib.h"

#define 		private_key_icbc 	"./file/icbc/private_key.pem"

using namespace std;

int bankTradePrecreate(bankTradeRequest* request, bankTradeResponse* response)
{
    //********************************************加载私钥************************************
    BIO *key = BIO_new(BIO_s_file());
    BIO_read_filename(key, private_key_icbc);
    if(key == NULL)
    {
        perror("open key file error");
        return -1;
    }
    RSA *p_rsa = PEM_read_bio_RSAPrivateKey(key, NULL, NULL, NULL);
    if(p_rsa == NULL)
    {
        ERR_print_errors_fp(stdout);
        return -2;
    }

    //********************************************组装数据************************************
    time_t tt = time(NULL);
    tm* t = localtime(&tt);
    char timestamp[20] = {0};
    char date[12] = {0};
    char time[12] = {0};
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    sprintf(date, "%04d%02d%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
    sprintf(time, "%02d%02d%02d", t->tm_hour, t->tm_min, t->tm_sec);

    request->out_trade_no += date;
    request->out_trade_no += time;

    request->msg_id += date;
    request->msg_id += time;

    request->timestamp = timestamp;	//"2017-11-12 16:22:14";
    request->trade_date = date;		//"20171130";
    request->trade_time = time;		// 162214

    map <string, string> bizContentMap;
    bizContentMap.insert(make_pair("attach", request->attach));
    bizContentMap.insert(make_pair("mer_id", request->mer_id));
    bizContentMap.insert(make_pair("store_code", request->store_code));
    bizContentMap.insert(make_pair("tporder_create_ip", request->tporder_create_ip));
    bizContentMap.insert(make_pair("out_trade_no", request->out_trade_no));
    bizContentMap.insert(make_pair("order_amt", request->order_amt));
    bizContentMap.insert(make_pair("trade_date", request->trade_date));
    bizContentMap.insert(make_pair("trade_time", request->trade_time));
    bizContentMap.insert(make_pair("pay_expire", "1200"));	// 二维码有效期，单位：秒，必须小于24小时
    //bizContentMap.insert(make_pair("sp_flag", "0"));
    bizContentMap.insert(make_pair("notify_flag", "0"));
    //bizContentMap.insert(make_pair("notify_url", "127.0.0.1"));

    string biz_content = "{";
    for (map <string, string>::iterator iter=bizContentMap.begin(); iter!=bizContentMap.end(); iter++)
    {
            biz_content += "\"" + (iter->first + "\":\"" + iter->second + "\"" + ",");
    }
    biz_content = biz_content.substr(0, biz_content.length()-1);
    biz_content += "}";
    //cout << biz_content << endl;

    string format = "json";
    string charset = "UTF-8";
    string sign_type = "RSA2";

    map <string, string> paramMap;
    paramMap.insert(make_pair("app_id", request->app_id));
    paramMap.insert(make_pair("msg_id", request->msg_id));
    paramMap.insert(make_pair("format", format));
    paramMap.insert(make_pair("charset", charset));
    paramMap.insert(make_pair("sign_type", sign_type));
    paramMap.insert(make_pair("timestamp", request->timestamp));
    paramMap.insert(make_pair("biz_content", biz_content));

    string senddata;
    for (map <string, string>::iterator iter=paramMap.begin(); iter!=paramMap.end(); iter++)
    {
        senddata += (iter->first + "=" + iter->second + "&");
    }
    senddata = senddata.substr(0, senddata.length()-1);
    //cout << senddata << endl;

    //********************************************私钥签名************************************
    string method = "/api/qrcode/V2/generate";
    string sigl = method + "?" + senddata;
    //cout << "sigl:"  << sigl << endl;

    // 生成 SHA256 散列值
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)sigl.c_str(), sigl.length(), md);
    //printHash(md, SHA256_DIGEST_LENGTH);

    // RSA2加密签名
    unsigned char sig[256] = {0};
    unsigned int siglen = sizeof(sig);
    int ret = RSA_sign(NID_sha256, md, SHA256_DIGEST_LENGTH, sig, &siglen, p_rsa);

    // base64加密
    unsigned char base64[2048] = {0};
    EVP_EncodeBlock(base64, (unsigned char *)sig, 256);
    //printf("base64:%s\n", base64);

    // url中有些特殊字符不能发送，所以需要urlencode转码
    char *strEncode = URLEncode((char *)base64, strlen((char *)base64));
    //printf("URLEncode:%s\n", strEncode);

    string sendpake = "sign=";
    sendpake += strEncode;
    sendpake += "&";
    sendpake += senddata;
    cout << sendpake << endl;

    //********************************************发送数据************************************
    string url = "https://" + request->url + method;
    cout << "url:" << url << endl;

    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Posts(url, sendpake, recvdata, NULL);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            // 瓒呮椂0.5s*3
            printf("____________瓒呮椂_____________\n");
            if (++nTimeoutCnt > 3)
            {
                    nTimeoutCnt = 0;
                    return -4;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            // 缃戠粶涓嶉€?
            nTimeoutCnt = 0;
            return -5;
        }
        else
        {
            // 鏈€氳
            nTimeoutCnt = 0;
            return -6;
        }
        usleep(500*1000);
    }

    //*******************************************接收数据***********************************************
    if (recvdata.length() <= 0)
            return -7;
    //printf(" Parma Data\n");

    cJSON *json = cJSON_Parse(recvdata.c_str());
    cJSON *arrayhead = cJSON_GetObjectItem(json, "response_biz_content");
    cJSON *itemname = cJSON_GetObjectItem(arrayhead, "return_code");
    //*
    if (itemname != NULL)  // 当返回400026时，是Int型，其他的是String型
            response->return_code = itemname->valueint;
    printf("response->return_code:%d\n", response->return_code);
    if (response->return_code != 0)
    {
        cJSON_Delete(json);
        return -1;
    }

    itemname = cJSON_GetObjectItem(arrayhead, "return_msg");
    if (itemname != NULL)  // 当返回400026时，是Int型，其他的是String型
    {
        response->return_msg = itemname->valuestring;
        cout << "return_msg:" << response->return_msg << endl;
        if (response->return_msg.compare("") != 0)
        {
            cJSON_Delete(json);
            return -2;
        }
    }

    /*
    itemname = cJSON_GetObjectItem(arrayhead, "return_msg");
    if (itemname != NULL)
            response->return_msg = itemname->valuestring;
    cout << "return_msg:" << response->return_msg << endl;
    itemname = cJSON_GetObjectItem(arrayhead, "success");
    if (itemname != NULL)
            response->return_staus = itemname->valuestring;
    cout << "success:" << response->code << endl;
    if (response->return_staus.compare("true") != 0)
            return -1;
    */
    itemname = cJSON_GetObjectItem(arrayhead, "qrcode");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "qrcode:" << response->qr_code << endl;
    cJSON_Delete(json);
    return 1;
}
int bankTradePay(bankTradeRequest* request, bankTradeResponse* response)
{
    //********************************************加载私钥************************************
    BIO *key = BIO_new(BIO_s_file());
    BIO_read_filename(key, private_key_icbc);
    if(key == NULL)
    {
        perror("open key file error");
        return -1;
    }
    RSA *p_rsa = PEM_read_bio_RSAPrivateKey(key, NULL, NULL, NULL);
    if(p_rsa == NULL)
    {
        ERR_print_errors_fp(stdout);
        return -2;
    }

    //********************************************组装数据************************************
    time_t tt = time(NULL);
    tm* t = localtime(&tt);
    char timestamp[20] = {0};
    char date[12] = {0};
    char time[12] = {0};
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    sprintf(date, "%04d%02d%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
    sprintf(time, "%02d%02d%02d", t->tm_hour, t->tm_min, t->tm_sec);

    request->out_trade_no += date;
    request->out_trade_no += time;

    request->msg_id += date;
    request->msg_id += time;

    request->timestamp = timestamp;	//"2017-11-12 16:22:14";
    request->trade_date = date;		//"20171130";
    request->trade_time = time;		// 162214

    map <string, string> bizContentMap;
    bizContentMap.insert(make_pair("attach", request->attach));
    bizContentMap.insert(make_pair("mer_id", request->mer_id));
    bizContentMap.insert(make_pair("qr_code", request->auth_code));
    bizContentMap.insert(make_pair("out_trade_no", request->out_trade_no));
    bizContentMap.insert(make_pair("order_amt", request->order_amt));
    bizContentMap.insert(make_pair("trade_date", request->trade_date));
    bizContentMap.insert(make_pair("trade_time", request->trade_time));

    string biz_content = "{";
    for (map <string, string>::iterator iter=bizContentMap.begin(); iter!=bizContentMap.end(); iter++)
    {
            biz_content += "\"" + (iter->first + "\":\"" + iter->second + "\"" + ",");
    }
    biz_content = biz_content.substr(0, biz_content.length()-1);
    biz_content += "}";
    //cout << biz_content << endl;

    string format = "json";
    string charset = "UTF-8";
    string sign_type = "RSA2";

    map <string, string> paramMap;
    paramMap.insert(make_pair("app_id", request->app_id));
    paramMap.insert(make_pair("msg_id", request->msg_id));
    paramMap.insert(make_pair("format", format));
    paramMap.insert(make_pair("charset", charset));
    paramMap.insert(make_pair("sign_type", sign_type));
    paramMap.insert(make_pair("timestamp", request->timestamp));
    paramMap.insert(make_pair("biz_content", biz_content));

    string senddata;
    for (map <string, string>::iterator iter=paramMap.begin(); iter!=paramMap.end(); iter++)
    {
            senddata += (iter->first + "=" + iter->second + "&");
    }
    senddata = senddata.substr(0, senddata.length()-1);
    //cout << senddata << endl;

    //********************************************私钥签名************************************
    string method = "/api/qrcode/V2/pay";
    string sigl = method + "?" + senddata;
    //cout << "sigl:"  << sigl << endl;

    // 生成 SHA256 散列值
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)sigl.c_str(), sigl.length(), md);
    //printHash(md, SHA256_DIGEST_LENGTH);

    // RSA2加密签名
    unsigned char sig[256] = {0};
    unsigned int siglen = sizeof(sig);
    int ret = RSA_sign(NID_sha256, md, SHA256_DIGEST_LENGTH, sig, &siglen, p_rsa);

    // base64加密
    unsigned char base64[2048] = {0};
    EVP_EncodeBlock(base64, (unsigned char *)sig, 256);
    //printf("base64:%s\n", base64);

    // url中有些特殊字符不能发送，所以需要urlencode转码
    char *strEncode = URLEncode((char *)base64, strlen((char *)base64));
    //printf("URLEncode:%s\n", strEncode);

    string sendpake = "sign=";
    sendpake += strEncode;
    sendpake += "&";
    sendpake += senddata;
    //cout << sendpake << endl;

    //********************************************发送数据************************************
    string url = "https://" + request->url + method;
    //cout << "url:" << url << endl;

    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Posts(url, sendpake, recvdata, NULL);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            // 瓒呮椂0.5s*3
            printf("____________瓒呮椂_____________\n");
            if (++nTimeoutCnt > 3)
            {
                    nTimeoutCnt = 0;
                    return -4;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            // 缃戠粶涓嶉€?
            nTimeoutCnt = 0;
            return -5;
        }
        else
        {
            // 鏈€氳
            nTimeoutCnt = 0;
            return -6;
        }
        usleep(500*1000);
    }

    //*******************************************接收数据***********************************************
    if (recvdata.length() <= 0)
            return -7;
    //printf(" Parma Data\n");

    cJSON *json = cJSON_Parse(recvdata.c_str());
    cJSON *arrayhead = cJSON_GetObjectItem(json, "response_biz_content");
    cJSON *itemname = cJSON_GetObjectItem(arrayhead, "return_code");

    if (itemname != NULL)  // 当返回400026时，是Int型，其他的是String型
            response->return_code = atoi(itemname->valuestring);
    printf("response->return_code:%d\n", response->return_code);
    if (response->return_code != 0)
    {
         cJSON_Delete(json);
         return -1;
    }

    itemname = cJSON_GetObjectItem(arrayhead, "return_msg");
    if (itemname != NULL)  // 当返回400026时，是Int型，其他的是String型
    {
        response->return_msg = itemname->valuestring;
        cout << "return_msg:" << response->return_msg << endl;
        if (response->return_msg.compare("") != 0)
        {
            cJSON_Delete(json);
            return -2;
        }
    }

    itemname = cJSON_GetObjectItem(arrayhead, "out_trade_no");
    if (itemname != NULL)
        response->tradeNo = itemname->valuestring;
    cout << "out_trade_no:" << response->tradeNo << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "pay_status");
    if (itemname != NULL)
        response->code = itemname->valuestring;
    cout << "pay_status:" << response->code << endl;
    if (response->code.compare("1") == 0)
    {
         cJSON_Delete(json);
         return 1;
    }
    else if (response->code.compare("0") == 0)
    {
        cJSON_Delete(json);
        return 0;
    }
    cJSON_Delete(json);
    return -1;
}
int bankTradeQuery(bankTradeRequest* request, bankTradeResponse* response)
{
    //********************************************加载私钥************************************
    BIO *key = BIO_new(BIO_s_file());
    BIO_read_filename(key, private_key_icbc);
    if(key == NULL)
    {
        perror("open key file error");
        return -1;
    }
    RSA *p_rsa = PEM_read_bio_RSAPrivateKey(key, NULL, NULL, NULL);
    if(p_rsa == NULL)
    {
        ERR_print_errors_fp(stdout);
        return -2;
    }

    //********************************************组装数据************************************
    time_t tt = time(NULL);
    tm* t = localtime(&tt);
    char timestamp[20] = {0};
    char date[12] = {0};
    char time[12] = {0};
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    sprintf(date, "%04d%02d%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
    sprintf(time, "%02d%02d%02d", t->tm_hour, t->tm_min, t->tm_sec);

    request->msg_id = "170880384";				// 消息通讯唯一编号，每次调用独立生成，APP级唯一
    request->msg_id += date;
    request->msg_id += time;
    request->timestamp = timestamp;				// "2017-11-12 16:22:14";

    map <string, string> bizContentMap;
    bizContentMap.insert(make_pair("mer_id", request->mer_id));
    bizContentMap.insert(make_pair("out_trade_no", request->out_trade_no));

    string biz_content = "{";
    for (map <string, string>::iterator iter=bizContentMap.begin(); iter!=bizContentMap.end(); iter++)
    {
            biz_content += "\"" + (iter->first + "\":\"" + iter->second + "\"" + ",");
    }
    biz_content = biz_content.substr(0, biz_content.length()-1);
    biz_content += "}";
    //cout << biz_content << endl;

    string format = "json";
    string charset = "UTF-8";
    string sign_type = "RSA2";

    map <string, string> paramMap;
    paramMap.insert(make_pair("app_id", request->app_id));
    paramMap.insert(make_pair("msg_id", request->msg_id));
    paramMap.insert(make_pair("format", format));
    paramMap.insert(make_pair("charset", charset));
    paramMap.insert(make_pair("sign_type", sign_type));
    paramMap.insert(make_pair("timestamp", request->timestamp));
    paramMap.insert(make_pair("biz_content", biz_content));

    string senddata;
    for (map <string, string>::iterator iter=paramMap.begin(); iter!=paramMap.end(); iter++)
    {
            senddata += (iter->first + "=" + iter->second + "&");
    }
    senddata = senddata.substr(0, senddata.length()-1);
    //cout << senddata << endl;

    //********************************************私钥签名************************************
    string method = "/api/qrcode/V2/query";
    string sigl = method + "?" + senddata;
    //cout << "sigl:"  << sigl << endl;

    // 生成 SHA256 散列值
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)sigl.c_str(), sigl.length(), md);
    //printHash(md, SHA256_DIGEST_LENGTH);

    // RSA2加密签名
    unsigned char sig[256] = {0};
    unsigned int siglen = sizeof(sig);
    int ret = RSA_sign(NID_sha256, md, SHA256_DIGEST_LENGTH, sig, &siglen, p_rsa);

    // base64加密
    unsigned char base64[2048] = {0};
    EVP_EncodeBlock(base64, (unsigned char *)sig, 256);
    //printf("base64:%s\n", base64);

    // url中有些特殊字符不能发送，所以需要urlencode转码
    char *strEncode = URLEncode((char *)base64, strlen((char *)base64));
    //printf("URLEncode:%s\n", strEncode);

    string sendpake = "sign=";
    sendpake += strEncode;
    sendpake += "&";
    sendpake += senddata;
    cout << sendpake << endl;

    //********************************************发送数据************************************
    string url = "https://" + request->url + method;
    cout << "url:" << url << endl;

    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Posts(url, sendpake, recvdata, NULL);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            // 瓒呮椂0.5s*3
            printf("____________瓒呮椂_____________\n");
            if (++nTimeoutCnt > 3)
            {
                    nTimeoutCnt = 0;
                    return -4;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            // 缃戠粶涓嶉€?
            nTimeoutCnt = 0;
            return -5;
        }
        else
        {
            // 鏈€氳
            nTimeoutCnt = 0;
            return -6;
        }
        usleep(500*1000);
    }

    //*******************************************接收数据***********************************************
    if (recvdata.length() <= 0)
            return -7;
    //printf(" Parma Data\n");

    cJSON *json = cJSON_Parse(recvdata.c_str());
    cJSON *arrayhead = cJSON_GetObjectItem(json, "response_biz_content");
    cJSON *itemname = cJSON_GetObjectItem(arrayhead, "return_code");
    //*
    if (itemname != NULL)  // 当返回400026时，是Int型，其他的是String型
            response->return_code = itemname->valueint;
    printf("response->return_code:%d\n", response->return_code);
    if (response->return_code != 0)
    {
        cJSON_Delete(json);
        return -1;
    }

    itemname = cJSON_GetObjectItem(arrayhead, "return_msg");
    if (itemname != NULL)  // 当返回400026时，是Int型，其他的是String型
    {
        response->return_msg = itemname->valuestring;
        cout << "return_msg:" << response->return_msg << endl;
        if (response->return_msg.compare("") != 0)
        {
            cJSON_Delete(json);
            return -2;
        }
    }

    /*
    itemname = cJSON_GetObjectItem(arrayhead, "success");
    if (itemname != NULL)
            response->code = itemname->valuestring;
    cout << "success:" << response->code << endl;
    if (response->code.compare("TRUE") != 0)
            return -1;
    itemname = cJSON_GetObjectItem(arrayhead, "return_msg");
    if (itemname != NULL)
            response->msg = itemname->valuestring;
    cout << "return_msg:" << response->msg << endl;
    */
    itemname = cJSON_GetObjectItem(arrayhead, "pay_status");
    if (itemname != NULL)
            response->code = itemname->valuestring;
    cout << "pay_status:" << response->code << endl;
    if (response->code.compare("1") != 0)
    {
        cJSON_Delete(json);
            return -1;
    }
    itemname = cJSON_GetObjectItem(arrayhead, "out_trade_no");
    if (itemname != NULL)
            response->tradeNo = itemname->valuestring;
    cout << "out_trade_no:" << response->tradeNo << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "card_no");
    if (itemname != NULL)
            response->card_no = itemname->valuestring;
    cout << "card_no:" << response->card_no << endl;
    // 订单总金额，单位：分
    itemname = cJSON_GetObjectItem(arrayhead, "total_amt");
    if (itemname != NULL)
            response->total_amt = itemname->valuestring;
    cout << "total_amt:" << response->total_amt << endl;
    // 积分抵扣金额
    itemname = cJSON_GetObjectItem(arrayhead, "point_amt");
    if (itemname != NULL)
            response->point_amt = itemname->valuestring;
    cout << "point_amt:" << response->point_amt << endl;
    // 电子券抵扣金额
    itemname = cJSON_GetObjectItem(arrayhead, "ecoupon_amt");
    if (itemname != NULL)
            response->ecoupon_amt = itemname->valuestring;
    cout << "ecoupon_amt:" << response->ecoupon_amt << endl;
    // 优惠立减金额
    itemname = cJSON_GetObjectItem(arrayhead, "mer_disc_amt");
    if (itemname != NULL)
            response->mer_disc_amt = itemname->valuestring;
    cout << "mer_disc_amt:" << response->mer_disc_amt << endl;
    // 优惠券金额
    itemname = cJSON_GetObjectItem(arrayhead, "coupon_amt");
    if (itemname != NULL)
            response->coupon_amt = itemname->valuestring;
    cout << "coupon_amt:" << response->coupon_amt << endl;
    // 银行补贴金额
    itemname = cJSON_GetObjectItem(arrayhead, "bank_disc_amt");
    if (itemname != NULL)
            response->bank_disc_amt = itemname->valuestring;
    cout << "bank_disc_amt:" << response->bank_disc_amt << endl;
    // 用户实际扣减金额
    itemname = cJSON_GetObjectItem(arrayhead, "payment_amt");
    if (itemname != NULL)
            response->payment_amt = itemname->valuestring;
    cout << "payment_amt:" << response->payment_amt << endl;
    // 总优惠金额，其值=优惠立减金额（商户部分）+银行补贴金额
    itemname = cJSON_GetObjectItem(arrayhead, "total_disc_amt");
    if (itemname != NULL)
            response->total_disc_amt = itemname->valuestring;
    cout << "total_disc_amt:" << response->total_disc_amt << endl;

    cJSON_Delete(json);
    return 1;
}
int bankTradeCancel(bankTradeRequest* request, bankTradeResponse* response)
{
    return 1;
}
int bankTradeRefund(bankTradeRequest* request, bankTradeResponse* response)
{
    //********************************************加载私钥************************************
    BIO *key = BIO_new(BIO_s_file());
    BIO_read_filename(key, private_key_icbc);
    if(key == NULL)
    {
        perror("open key file error");
        return -1;
    }
    RSA *p_rsa = PEM_read_bio_RSAPrivateKey(key, NULL, NULL, NULL);
    if(p_rsa == NULL)
    {
        ERR_print_errors_fp(stdout);
        return -2;
    }

    //********************************************组装数据************************************
    time_t tt = time(NULL);
    tm* t = localtime(&tt);
    char timestamp[20] = {0};
    char date[12] = {0};
    char time[12] = {0};
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    sprintf(date, "%04d%02d%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
    sprintf(time, "%02d%02d%02d", t->tm_hour, t->tm_min, t->tm_sec);

    request->reject_no += date;
    request->reject_no += time;

    request->msg_id += date;
    request->msg_id += time;

    request->timestamp = timestamp;				// "2017-11-12 16:22:14";

    map <string, string> bizContentMap;
    bizContentMap.insert(make_pair("mer_id", request->mer_id));
    bizContentMap.insert(make_pair("out_trade_no", request->out_trade_no));
    bizContentMap.insert(make_pair("reject_no", request->reject_no));
    bizContentMap.insert(make_pair("reject_amt", request->reject_amt));

    string biz_content = "{";
    for (map <string, string>::iterator iter=bizContentMap.begin(); iter!=bizContentMap.end(); iter++)
    {
            biz_content += "\"" + (iter->first + "\":\"" + iter->second + "\"" + ",");
    }
    biz_content = biz_content.substr(0, biz_content.length()-1);
    biz_content += "}";
    cout << biz_content << endl;

    string format = "json";
    string charset = "UTF-8";
    string sign_type = "RSA2";

    map <string, string> paramMap;
    paramMap.insert(make_pair("app_id", request->app_id));
    paramMap.insert(make_pair("msg_id", request->msg_id));
    paramMap.insert(make_pair("format", format));
    paramMap.insert(make_pair("charset", charset));
    paramMap.insert(make_pair("sign_type", sign_type));
    paramMap.insert(make_pair("timestamp", request->timestamp));
    paramMap.insert(make_pair("biz_content", biz_content));

    string senddata;
    for (map <string, string>::iterator iter=paramMap.begin(); iter!=paramMap.end(); iter++)
    {
            senddata += (iter->first + "=" + iter->second + "&");
    }
    senddata = senddata.substr(0, senddata.length()-1);
    cout << senddata << endl;

    //********************************************私钥签名************************************
    string method = "/api/qrcode/V2/reject";
    string sigl = method + "?" + senddata;
    cout << "sigl:"  << sigl << endl;

    // 生成 SHA256 散列值
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)sigl.c_str(), sigl.length(), md);
    printHash(md, SHA256_DIGEST_LENGTH);

    // RSA2加密签名
    unsigned char sig[256] = {0};
    unsigned int siglen = sizeof(sig);
    int ret = RSA_sign(NID_sha256, md, SHA256_DIGEST_LENGTH, sig, &siglen, p_rsa);

    // base64加密
    unsigned char base64[2048] = {0};
    EVP_EncodeBlock(base64, (unsigned char *)sig, 256);
    //printf("base64:%s\n", base64);

    // url中有些特殊字符不能发送，所以需要urlencode转码
    char *strEncode = URLEncode((char *)base64, strlen((char *)base64));
    //printf("URLEncode:%s\n", strEncode);

    string sendpake = "sign=";
    sendpake += strEncode;
    sendpake += "&";
    sendpake += senddata;
    cout << sendpake << endl;

    //********************************************发送数据************************************
    string url = "https://" + request->url + method;
    cout << "url:" << url << endl;

    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Posts(url, sendpake, recvdata, NULL);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            // 瓒呮椂0.5s*3
            printf("____________瓒呮椂_____________\n");
            if (++nTimeoutCnt > 3)
            {
                    nTimeoutCnt = 0;
                    return -4;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            // 缃戠粶涓嶉€?
            nTimeoutCnt = 0;
            return -5;
        }
        else
        {
            // 鏈€氳
            nTimeoutCnt = 0;
            return -6;
        }
        usleep(500*1000);
    }

    //*******************************************接收数据***********************************************
    if (recvdata.length() <= 0)
            return -7;
    //printf(" Parma Data\n");

    cJSON *json = cJSON_Parse(recvdata.c_str());
    cJSON *arrayhead = cJSON_GetObjectItem(json, "response_biz_content");
    cJSON *itemname = cJSON_GetObjectItem(arrayhead, "return_code");
    response->return_code = 1;
    if (itemname != NULL)
    {
        response->return_code = atoi(itemname->valuestring);
        printf("response->return_code:%d\n", response->return_code);
    }
    if (response->return_code == 0)
    {
        cJSON_Delete(json);
        return 1;
    }
    else
    {
        cJSON_Delete(json);
        return -1;
    }

    /*itemname = cJSON_GetObjectItem(arrayhead, "success");
    if (itemname != NULL)
     {
         response->code = itemname->valuestring;
            cout << "success:" << response->code << endl;
     }
    if (response->code.compare("TRUE") != 0)
            return -1;
    itemname = cJSON_GetObjectItem(arrayhead, "return_msg");
    if (itemname != NULL)
            response->msg = itemname->valuestring;
    cout << "return_msg:" << response->msg << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "out_trade_no");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "out_trade_no:" << response->qr_code << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "card_no");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "card_no:" << response->qr_code << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "reject_no");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "reject_no:" << response->qr_code << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "real_reject_amt");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "real_reject_amt:" << response->qr_code << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "reject_amt");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "reject_amt:" << response->qr_code << endl;
*/
    cJSON_Delete(json);
    return 1;
}
int bankTradeRefundQuery(bankTradeRequest* request, bankTradeResponse* response)
{
    //********************************************加载私钥************************************
    BIO *key = BIO_new(BIO_s_file());
    BIO_read_filename(key, private_key_icbc);
    if(key == NULL)
    {
        perror("open key file error");
        return -1;
    }
    RSA *p_rsa = PEM_read_bio_RSAPrivateKey(key, NULL, NULL, NULL);
    if(p_rsa == NULL)
    {
        ERR_print_errors_fp(stdout);
        return -2;
    }

    //********************************************组装数据************************************
    time_t tt = time(NULL);
    tm* t = localtime(&tt);
    char timestamp[20] = {0};
    char date[12] = {0};
    char time[12] = {0};
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
    sprintf(date, "%04d%02d%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
    sprintf(time, "%02d%02d%02d", t->tm_hour, t->tm_min, t->tm_sec);

    request->msg_id = "170880384";				// 消息通讯唯一编号，每次调用独立生成，APP级唯一
    request->msg_id += date;
    request->msg_id += time;
    request->timestamp = timestamp;				// "2017-11-12 16:22:14";

    map <string, string> bizContentMap;
    bizContentMap.insert(make_pair("mer_id", request->mer_id));
    bizContentMap.insert(make_pair("out_trade_no", request->out_trade_no));
    bizContentMap.insert(make_pair("reject_no", request->reject_no));

    string biz_content = "{";
    for (map <string, string>::iterator iter=bizContentMap.begin(); iter!=bizContentMap.end(); iter++)
    {
            biz_content += "\"" + (iter->first + "\":\"" + iter->second + "\"" + ",");
    }
    biz_content = biz_content.substr(0, biz_content.length()-1);
    biz_content += "}";
    cout << biz_content << endl;

    string format = "json";
    string charset = "UTF-8";
    string sign_type = "RSA2";

    map <string, string> paramMap;
    paramMap.insert(make_pair("app_id", request->app_id));
    paramMap.insert(make_pair("msg_id", request->msg_id));
    paramMap.insert(make_pair("format", format));
    paramMap.insert(make_pair("charset", charset));
    paramMap.insert(make_pair("sign_type", sign_type));
    paramMap.insert(make_pair("timestamp", request->timestamp));
    paramMap.insert(make_pair("biz_content", biz_content));

    string senddata;
    for (map <string, string>::iterator iter=paramMap.begin(); iter!=paramMap.end(); iter++)
    {
            senddata += (iter->first + "=" + iter->second + "&");
    }
    senddata = senddata.substr(0, senddata.length()-1);
    cout << senddata << endl;

    //********************************************私钥签名************************************
    string method = "/api/qrcode/V2/reject/query";
    string sigl = method + "?" + senddata;
    cout << "sigl:"  << sigl << endl;

    // 生成 SHA256 散列值
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)sigl.c_str(), sigl.length(), md);
    printHash(md, SHA256_DIGEST_LENGTH);

    // RSA2加密签名
    unsigned char sig[256] = {0};
    unsigned int siglen = sizeof(sig);
    int ret = RSA_sign(NID_sha256, md, SHA256_DIGEST_LENGTH, sig, &siglen, p_rsa);

    // base64加密
    unsigned char base64[2048] = {0};
    EVP_EncodeBlock(base64, (unsigned char *)sig, 256);
    //printf("base64:%s\n", base64);

    // url中有些特殊字符不能发送，所以需要urlencode转码
    char *strEncode = URLEncode((char *)base64, strlen((char *)base64));
    //printf("URLEncode:%s\n", strEncode);

    string sendpake = "sign=";
    sendpake += strEncode;
    sendpake += "&";
    sendpake += senddata;
    cout << sendpake << endl;

    //********************************************发送数据************************************
    string url = "https://" + request->url + method;
    cout << "url:" << url << endl;

    string recvdata;
    CHttpClient client;
    int nTimeoutCnt = 0;
    while(1)
    {
        int nPostsResult = client.Posts(url, sendpake, recvdata, NULL);
        if (nPostsResult == CURLE_OK)
        {
            nTimeoutCnt = 0;
            break;
        }
        else if (nPostsResult == CURLE_OPERATION_TIMEDOUT)
        {
            // 瓒呮椂0.5s*3
            printf("____________瓒呮椂_____________\n");
            if (++nTimeoutCnt > 3)
            {
                    nTimeoutCnt = 0;
                    return -4;
            }
            continue;
        }
        else if (nPostsResult == CURLE_COULDNT_RESOLVE_HOST)
        {
            // 缃戠粶涓嶉€?
            nTimeoutCnt = 0;
            return -5;
        }
        else
        {
            // 鏈€氳
            nTimeoutCnt = 0;
            return -6;
        }
        usleep(500*1000);
    }

    //*******************************************接收数据***********************************************
    if (recvdata.length() <= 0)
            return -7;
    printf(" Parma Data\n");

    cJSON *json = cJSON_Parse(recvdata.c_str());
    cJSON *arrayhead = cJSON_GetObjectItem(json, "response_biz_content");
    cJSON *itemname = cJSON_GetObjectItem(arrayhead, "return_code");
    //*
    if (itemname != NULL)  // 当返回400026时，是Int型，其他的是String型
            response->return_code = itemname->valueint;
    printf("response->return_code:%d\n", response->return_code);
    if (response->return_code != 0)
    {
        cJSON_Delete(json);
            return -1;
    }
    //*/
    /*
    if (itemname != NULL)
            response->return_code = itemname->valuestring;
    printf("response->return_code:%d\n", response->return_code);
    if (response->return_code.compare("0") != 0)
            return -1;
    */

    itemname = cJSON_GetObjectItem(arrayhead, "success");
    if (itemname != NULL)
            response->code = itemname->valuestring;
    cout << "success:" << response->code << endl;
    if (response->code.compare("TRUE") != 0)
    {
        cJSON_Delete(json);
            return -1;
    }
    itemname = cJSON_GetObjectItem(arrayhead, "return_msg");
    if (itemname != NULL)
            response->msg = itemname->valuestring;
    cout << "return_msg:" << response->msg << endl;
/*
    itemname = cJSON_GetObjectItem(arrayhead, "out_trade_no");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "out_trade_no:" << response->qr_code << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "card_no");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "card_no:" << response->qr_code << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "reject_no");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "reject_no:" << response->qr_code << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "real_reject_amt");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "real_reject_amt:" << response->qr_code << endl;

    itemname = cJSON_GetObjectItem(arrayhead, "reject_amt");
    if (itemname != NULL)
            response->qr_code = itemname->valuestring;
    cout << "reject_amt:" << response->qr_code << endl;
*/
    cJSON_Delete(json);
    return 1;
}
