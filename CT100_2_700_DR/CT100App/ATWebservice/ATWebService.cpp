#include <pthread.h>
#include "ATWebService.h"
#include "WebService.h"

static unsigned int m_nWebServerProcCnt = 0;

WebPublic webPublic;
CShareInfo 	*m_pWebInfo = NULL;

int ATHeartBeat()
{
    return HeartBeat(webPublic, m_pWebInfo);
}
int ATGetTime()
{
    int nRet = GetServerTime(webPublic, m_pWebInfo);
    return nRet;
}
int GetUserInfo()
{
    return GetUserInfo(webPublic, m_pWebInfo);
}
int GetPluInfo()
{
    return GetPluInfo(webPublic, m_pWebInfo);
}

int ATUploadOrder()
{
    int nRet = UploadSaleInfo(webPublic, m_pWebInfo);
    return nRet;
}

void *theadWebServerProc(void *p)
{
    printf("-------------------------------------------[theadWebServerProc]---------------\n");

    while(1)
    {     
        if (0 == m_pWebInfo->m_ShareSysSwitchInfo.WebSwitch)
        {
            continue;
        }
        if (ATHeartBeat() > 0)
        {
            usleep(1*1000*1000);
            if (ATGetTime() > 0)
            {
                break;
            }
        }
        usleep(5*1000*1000);
    }
    m_pWebInfo->m_WebserviceUpFlag = 1;
    int nRet = 0;
    while(1)
    {
        usleep(1*1000*1000);
        if (webPublic.userFlags == 1)
        {
            if(11 == GetUserInfo())
            {
                webPublic.pluFlags = 1;
            }
            webPublic.userFlags = 0;
            continue;
        }
        if (webPublic.pluFlags == 1)
        {
            nRet = GetPluInfo();
            if(nRet>0)
            {
                sprintf(m_pWebInfo->shareInfoSaleUi.message,"更新商户商品成功,获取%s条",nRet);
                m_pWebInfo->UpdateGui();
            }
            webPublic.pluFlags = 0;
            continue;
        }

        if(1 == m_pWebInfo->m_WebserviceUpFlag)
        {
            nRet = GetUserInfo();
            if(11 == nRet)
            {
                printf("update user info----------------\n");
                m_pWebInfo->ShowMessage("发现新商户版本，正在更新商户信息");
                usleep(1*1000*1000);
                if(GetUserInfo()>0)
                {
                    m_pWebInfo->ShowMessage("发现新商户版本，更新商户信息成功");
                }
                else{
                    m_pWebInfo->ShowMessage("发现新商户版本，更新商户信息失败");
                }
                usleep(1*1000*1000);
                m_pWebInfo->m_WebserviceUpFlag = 2;
            }
            else if(1 == nRet)
            {
                usleep(2*1000*1000);
                m_pWebInfo->m_WebserviceUpFlag = 0xFF;
            }
        }
        else if(2 == m_pWebInfo->m_WebserviceUpFlag)
        {
            m_pWebInfo->ShowMessage("发现新商户版本，正在更新商品信息");
            usleep(1*1000*1000);
            nRet = GetPluInfo();
            if(nRet>0)
            {
                sprintf(m_pWebInfo->shareInfoSaleUi.message,"更新商户商品成功,获取%d条",nRet);
            }
            else{
                m_pWebInfo->ShowMessage("发现新商户版本，更新商品信息失败");
            }
            m_pWebInfo->m_WebserviceUpFlag = 0xFF;
        }
        else if(9 == m_pWebInfo->m_WebserviceUpFlag)
        {
            ATUploadOrder();
            m_pWebInfo->m_WebserviceUpFlag = 0xFF;
        }
        else
        {
            //60s,自动上传交易
            if(++m_nWebServerProcCnt>= 60)
            {
                m_nWebServerProcCnt = 0;
                if (ATHeartBeat() > 0)
                {
                    usleep(1*1000*1000);
                    ATUploadOrder();
                }
            }
        }
    }
}

int ATWebServerInit(CShareInfo *m_pInfo)
{
    m_pWebInfo = m_pInfo;
    webPublic.pluFlags = 0;
    webPublic.userFlags = 0;

    if (m_pInfo->m_ShareNetInfo.RemoteServer.contains("http://", Qt::CaseSensitive))
    {
        webPublic.weburl = m_pWebInfo->m_ShareNetInfo.RemoteServer.toStdString() ;
    }
    else
    {
        webPublic.weburl = "http://" + m_pWebInfo->m_ShareNetInfo.RemoteServer.toStdString() ;
    }
    cout <<"-------------------------------------------webUrl = " << webPublic.weburl << endl;
    pthread_t WebserverId;

    int pthread = pthread_create(&WebserverId, NULL, theadWebServerProc, NULL);
    if (pthread)
    {
        perror("create theadWebServerProc thread");
    }
    pthread = pthread_detach(WebserverId);
    if (pthread) {
        perror("detach theadWebServerProc thread");
    }
}
