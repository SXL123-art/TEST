#include <pthread.h>
#include "JNLC_ATWebService.h"
#include "JNLC_WebService.h"

JNLC_WebPublic web_jnlc_Public;
CShareInfo 	*m_pJNLC_WebInfo = NULL;

int JNLC_ATGetSystemTime()
{
    return JNLC_GetServerSystemTime(web_jnlc_Public,m_pJNLC_WebInfo);
}

int JNLC_ATGetGoodsList()
{
    return JNLC_GetGoodsList(web_jnlc_Public,m_pJNLC_WebInfo);
}

int JNLC_ATEnterDataToScales()
{
    return JNLC_EnterDataToScales(web_jnlc_Public,m_pJNLC_WebInfo);
}

int JNLC_ATGetTraceCode()
{
    return JNLC_GetTraceCode(web_jnlc_Public,m_pJNLC_WebInfo);
}

int JNLC_ATPutTransData()
{
    return JNLC_PutTransData(web_jnlc_Public,m_pJNLC_WebInfo);
}

void *jnlc_theadWebServerProc(void *p)
{
    static unsigned char m_nWebServerProcCnt = 0;
    static unsigned int m_nWebServerProcEntCnt = 0;
    printf("---------------theadWebServerProc---------------\n");
    usleep(10*000*1000);//10s
    JNLC_ATGetSystemTime();
    usleep(1*1000*1000);
    JNLC_ATEnterDataToScales();
    while(1)
    {
        usleep(1*1000*1000);
        if (0 == m_pJNLC_WebInfo->m_ShareSysSwitchInfo.WebSwitch)
        {
                continue;
        }
        if(m_pJNLC_WebInfo->m_WebserviceUpFlag == 3)
        {
            //JNLC_ATGetGoodsList();
            m_pJNLC_WebInfo->m_WebserviceUpFlag = 0;
        }
        if(m_nWebServerProcEntCnt ++ > 1800 || m_pJNLC_WebInfo->m_WebserviceUpFlag == 2)
        {
            if(m_pJNLC_WebInfo->m_WebserviceUpFlag == 2)
            {
                m_pJNLC_WebInfo->m_WebserviceUpFlag = 0;
            }
            JNLC_ATEnterDataToScales();
            usleep(4*1000*1000);
            m_nWebServerProcEntCnt = 0;
        }
        if(++m_nWebServerProcCnt>= 30 || m_pJNLC_WebInfo->m_WebserviceUpFlag == 1)
        {
            m_nWebServerProcCnt = 0;
            if(m_pJNLC_WebInfo->m_WebserviceUpFlag == 1)
            {
                m_pJNLC_WebInfo->m_WebserviceUpFlag = 0;
            }
            JNLC_ATPutTransData();
        }
    }
}
int JNLC_ATWebServerInit(CShareInfo *m_pInfo)
{
    m_pJNLC_WebInfo = m_pInfo;
    web_jnlc_Public.WebUrl = "http://" ;
    web_jnlc_Public.WebUrl += m_pJNLC_WebInfo->m_ShareNetInfo.RemoteServer.toStdString() ;
    web_jnlc_Public.WebUrl += "/cts/inf/collect";
    cout <<"webUrl = " << web_jnlc_Public.WebUrl << endl;

    web_jnlc_Public.auth_id = "20181218113302837405";
    web_jnlc_Public.key = "yW9kzOWgSFfE4p9aZso7Rb3OLDhFz7nG";

    pthread_t WebserverId;

    int pthread = pthread_create(&WebserverId, NULL, jnlc_theadWebServerProc, NULL);
    if (pthread)
    {
        perror("create theadWebServerProc thread");
        //exit(EXIT_FAILURE);
    }
    pthread = pthread_detach(WebserverId);
    if (pthread) {

            perror("detach theadWebServerProc thread");
            //exit(EXIT_FAILURE);
    }

}
