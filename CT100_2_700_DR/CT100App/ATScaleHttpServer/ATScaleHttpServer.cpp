#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //for getopt, fork
#include <string.h>     //for strcat
//for http
#include <event.h>
#include <evhttp.h>

#include "ScaleHttpService.h"
#define MYHTTPD_SIGNATURE   "CAShttpd v 1.0"
#define ServerIPAddress            "0.0.0.0"
#define ServerIPPort               3207
#define ServerTimeOut              8

#include <iostream>
using namespace std;

CShareInfo *ShareinfoHttpServer = NULL;

int parsingInterfaceName(char *interfaceName)
{
    char *address = NULL;
    if(address != strstr(interfaceName,"/UpdataScaleTime"))
    {
            return 1;
    }
    else if(address != strstr(interfaceName,"/CheckOrGetScaleTime"))
    {
            return 2;
    }
    else if(address != strstr(interfaceName,"/UpdataScaleSystemInfo"))
    {
            return 3;
    }
    else if(address != strstr(interfaceName,"/GetScaleSystemInfo"))
    {
            return 4;
    }
    else if(address != strstr(interfaceName,"/UpdataScaleUserInfo"))
    {
            return 5;
    }
    else if(address != strstr(interfaceName,"/GetScaleUserInfo"))
    {
            return 6;
    }
    else if(address != strstr(interfaceName,"/UpdataScaleLoginUser"))
    {
            return 7;
    }
    else if(address != strstr(interfaceName,"/GetScaleLoginUser"))
    {
            return 8;
    }
    else if(address != strstr(interfaceName,"/UpdataNetParameter"))
    {
            return 9;
    }
    else if(address != strstr(interfaceName,"/GetNetParameter"))
    {
            return 10;
    }
    else if(address != strstr(interfaceName,"/UpdataScaleUserSwitch"))
    {
            return 11;
    }
    else if(address != strstr(interfaceName,"/GetScaleUserSwitch"))
    {
            return 12;
    }
    else if(address != strstr(interfaceName,"/UpdataScaleSystemSwitch"))
    {
            return 13;
    }
    else if(address != strstr(interfaceName,"/GetScaleSystemSwitch"))
    {
            return 14;
    }
    else if(address != strstr(interfaceName,"/UpdataPLUList"))
    {
            return 15;
    }
    else if(address != strstr(interfaceName,"/GetScalePluList"))
    {
            return 16;
    }
    else if(address != strstr(interfaceName,"/GetScaleSaleInfo"))
    {
            return 17;
    }
    else if(address != strstr(interfaceName,"/UpdataGoodsHotKey"))
    {
            return 18;
    }
    else if(address != strstr(interfaceName,"/GetGoodsHotKey"))
    {
            return 19;
    }
    else if(address != strstr(interfaceName,"/UpdataScaleProgram"))
    {
            return 20;
    }
    else if(address != strstr(interfaceName,"/PerformSQL"))
    {
            return 21;
    }

    return 0;
}
void ResponesError400(struct evhttp_request *req,struct evbuffer *buf)
{
        evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
        evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
        evhttp_add_header(req->output_headers, "Connection", "close");
        evhttp_send_reply(req, 400, "request invalid :Server not interface",buf);
}

//处理模块
void httpd_handler(struct evhttp_request *request, void *arg)
{
    const char *cmdtype;
    switch (evhttp_request_get_command(request))
    {
        case EVHTTP_REQ_GET: cmdtype = "GET"; break;
        case EVHTTP_REQ_POST: cmdtype = "POST"; break;
        case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
        case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
        case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
        case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
        case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
        case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
        case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
        default: cmdtype = "unknown"; break;
    }
    char *interface;
    interface = strdup((char*)evhttp_request_uri(request));
    printf("request:%s  interface:%s\n",cmdtype,interface);

    //获取POST方法的数据
    int requestLen = EVBUFFER_LENGTH(request->input_buffer);

    printf("requestLen:%d\n", requestLen);
    string requestBuf = "";
    char * tmp = NULL;
    if(requestLen>0)
    {
        char *requestData = (char *) EVBUFFER_DATA(request->input_buffer);
        tmp = (char *)malloc(requestLen + 1);
        memcpy(tmp,(const char*)requestData,requestLen);
    }
    if(tmp != NULL)
    {
        requestBuf = tmp;
        free(tmp);
        tmp = NULL;
    }

    //printf("requestBuf:%s\n", requestBuf.c_str());

    struct evbuffer *evOutPutBuf;
    evOutPutBuf = evbuffer_new();
    /*
    具体的：可以根据GET/POST的参数执行相应操作，然后将结果输出
    ...
    */

    //printf("-----------------------------------1\n");
    string sendData = "";
    switch(parsingInterfaceName(interface))
    {
        case 1:
                        UpdataScaleTime(ShareinfoHttpServer,sendData,requestBuf);break;
        case 2:
                        CheckOrGetScaleTime(ShareinfoHttpServer,sendData,requestBuf); break;
        case 3:
                        UpdataScaleSystemInfo(ShareinfoHttpServer,sendData,requestBuf);break;
        case 4:
                        GetScaleSystemInfo(ShareinfoHttpServer,sendData,requestBuf);break;
        case 5:
                        UpdataScaleUserInfo(ShareinfoHttpServer,sendData,requestBuf);break;
        case 6:
                        GetScaleUserInfo(ShareinfoHttpServer,sendData,requestBuf);break;
        case 7:
                        UpdataScaleLoginUser(ShareinfoHttpServer,sendData,requestBuf);break;
        case 8:
                        GetScaleLoginUser(ShareinfoHttpServer,sendData,requestBuf);break;
        case 9:
                        UpdataNetParameter(ShareinfoHttpServer,sendData,requestBuf);break;
        case 10:
                        GetNetParameter(ShareinfoHttpServer,sendData,requestBuf);break;
        case 11:
                        UpdataScaleUserSwitch(ShareinfoHttpServer,sendData,requestBuf);break;
        case 12:
                        GetScaleUserSwitch(ShareinfoHttpServer,sendData,requestBuf);break;
        case 13:
                        UpdataScaleSystemSwitch(ShareinfoHttpServer,sendData,requestBuf);break;
        case 14:
                        GetScaleSystemSwitch(ShareinfoHttpServer,sendData,requestBuf);break;
        case 15:
                        UpdataPLUList(ShareinfoHttpServer,sendData,requestBuf);break;
        case 16:
                        GetScalePluList(ShareinfoHttpServer,sendData,requestBuf);break;
        case 17:
                        GetScaleSaleInfo(ShareinfoHttpServer,sendData,requestBuf);break;
        case 18:
                        UpdataGoodsHotKey(ShareinfoHttpServer,sendData,requestBuf);break;
        case 19:
                        GetGoodsHotKey(ShareinfoHttpServer,sendData,requestBuf);break;
        case 20:
                        UpdataScaleProgram(ShareinfoHttpServer,sendData,requestBuf);break;
        case 21:
                        PerformSQL(ShareinfoHttpServer,sendData,requestBuf);break;
        default :
                ResponesError400(request,evOutPutBuf);
                evbuffer_free(evOutPutBuf);
                return ;
                //报请求错误，无该接口
    }
    free(interface);
    //printf("-----------------------------------3\n");
    /* 输出到客户端 */
    //HTTP header
    evhttp_add_header(request->output_headers, "Server", MYHTTPD_SIGNATURE);
    evhttp_add_header(request->output_headers, "Content-Type", "text/plain; charset=UTF-8");
    evhttp_add_header(request->output_headers, "Connection", "close");
    //输出的内容

    evbuffer_add(evOutPutBuf,sendData.c_str(),sendData.length());
    //int len = evbuffer_get_length(evOutPutBuf);
    //printf("len:%d\n", len);
    evhttp_send_reply(request, HTTP_OK, "OK", evOutPutBuf);

    //evbuffer_drain(evOutPutBuf,sendData.length());

    evbuffer_free(evOutPutBuf);
}
void *theadScaleHttpServer(void* p)
{
    printf("-------------------------------------------[theadScaleHttpServer]-----%s\n",ShareinfoHttpServer->m_ShareNetInfo.Ip.toStdString().c_str());
    //初始化event API
    struct event_base * base = event_init();
    if(base == NULL)
    {
        printf("-------------------------------------------event_init failure\n");
        //exit(EXIT_FAILURE);
        return 0;
    }
    //创建一个http server
    struct evhttp *httpd;
    httpd = evhttp_start(ShareinfoHttpServer->m_ShareNetInfo.Ip.toStdString().c_str(), ServerIPPort);
    if(httpd == NULL)
    {
        printf("-------------------------------------------evhttp_start failure\n");
        event_base_dispatch(base);
        //exit(EXIT_FAILURE);
        return 0;
    }
    evhttp_set_timeout(httpd,ServerTimeOut);
    //指定generic callback
    evhttp_set_gencb(httpd, httpd_handler, NULL);
    //也可以为特定的URI指定callback
    //evhttp_set_cb(httpd, "/", specific_handler, NULL);

    //循环处理events
    event_dispatch();

    evhttp_free(httpd);
    exit(EXIT_FAILURE);
    return 0;
    //printf("-----------------------------ScaleHttpServerInit end\n");

}

int ScaleHttpServerInit(CShareInfo *m_pInfo)
 {
     ShareinfoHttpServer = m_pInfo;
     pthread_t theadHttpId;
     int pthread = pthread_create(&theadHttpId, NULL, theadScaleHttpServer, NULL);
     if (pthread)
     {
         errno = pthread;
         perror("create theadScaleHttpServer thread");
         //exit(EXIT_FAILURE);
     }
     pthread = pthread_detach(theadHttpId);
     if (pthread) {
             errno = pthread;
             perror("detach theadScaleHttpServer thread");
             //exit(EXIT_FAILURE);
     }
 }


