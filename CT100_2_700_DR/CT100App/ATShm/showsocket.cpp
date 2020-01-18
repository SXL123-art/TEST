#include "showsocket.h"
#include "../ATLib/ATLib.h"

#define DEFAULTPORT 3200
#define DEFAULTIP "192.168.1.108"
#define SEBDBUFLEN   1024*1
char pSendBuf[SEBDBUFLEN]= {0x00};

void* theadClientSend(void* pThis)
{
    showSocket* pMy = (showSocket*)pThis;
    while(true)
    {
        usleep(100*1000);

        //网络连接状态
        if(pMy->connected == false){
            continue;
        }

        int sendlen = strlen((char *)pSendBuf);
        if(sendlen <= 0)
        {
            continue;
        }

        pthread_mutex_lock (&pMy->mutex);
        int r = send(pMy->m_socket, (char *)pSendBuf,sendlen , 0);
        pthread_mutex_unlock (&pMy->mutex);
        if(r == -1){
            printf("\n XXXXXXXXX  [send error]  XXXXXXXXX!\n");
            shutdown(pMy->m_socket, 0);
            close(pMy->m_socket);
            pMy->connected = false;
        }
        memset(pSendBuf,0x00,SEBDBUFLEN);
    }
}

void* TryRunClient_local(void *pThis)
{
    showSocket* pMy = (showSocket*)pThis;

    pthread_t idSend;
    pthread_create(&idSend,	NULL, theadClientSend,	pThis);

    struct sockaddr_in addr;
    printf("TryRunClient_local ip :%s\n",pMy->IPAddress.toStdString().c_str());
    while(true)
    {
        if(pMy->connected == true)
        {
            usleep(3000*1000);
            continue;
        }

        bzero( &addr, sizeof( addr ));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(pMy->IPAddress.toStdString().c_str());
        addr.sin_port = htons(DEFAULTPORT);

        if ((pMy->m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("----------------socket fail\n");
            return (void*)0;
        }

        int TimeOut;
        TimeOut = 5000;
        setsockopt(pMy->m_socket,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut));//设置发送超时
        TimeOut = 1000;
        setsockopt(pMy->m_socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));//设置接收超时

        int nBufLen=1*1024;//设置为1K
        setsockopt(pMy->m_socket, SOL_SOCKET,SO_RCVBUF,(const char*)&nBufLen,sizeof(int));
        // 设置发送缓冲区
        nBufLen=1*1024;
        setsockopt(pMy->m_socket, SOL_SOCKET,SO_SNDBUF,(const char*)&nBufLen,sizeof(int));

        int res = connect(pMy->m_socket, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
        //printf("\nSOCKET conncet    ...............................[res]:%d\n",res);
        if (res == 0){
            if(pMy->connected == false)
            {
                pMy->connected = true;
                printf("\nSOCKET conncet    ...............................[Y]\n");
            }
        }else if(res == -1){
            if(pMy->connected == true)
            {
                pMy->connected = false;
                printf("\nSOCKET conncet    ...............................[X]\n");
            }
            close(pMy->m_socket);
        }
        usleep(3000*1000);
    }
}

showSocket::showSocket(QString ip)
{
    pthread_mutex_init(&mutex, NULL);
    IPAddress = ip;
    connected = false;
    pthread_t idSend;
    pthread_create(&idSend,NULL, TryRunClient_local,this);
    usleep(100*1000);
}
showSocket::~showSocket()
{

}

void showSocket::updateSale(stTabSaleUi* p)
{
    pthread_mutex_lock (&mutex);
    memset(pSendBuf,0x00,SEBDBUFLEN);
    char strPlu[64] = {0};
    ATLib_u2g(p->plu,strlen(p->plu),strPlu,64);

    sprintf(pSendBuf, "%s,%d,%d,%s,%s,%s,%s,%s", "cmd1", 0, p->saleMode,strPlu,p->weight,p->unitprice,p->totalprice,  "@#$");
    pthread_mutex_unlock (&mutex);
}
void showSocket::updatePay(stTabPayUi* p)
{
    pthread_mutex_lock (&mutex);
    memset(pSendBuf,0x00,SEBDBUFLEN);

    char payMode[32] = {0};
    ATLib_u2g(p->payMode,strlen(p->payMode),payMode,32);

    sprintf(pSendBuf, "%s,%s,%s,%s,%s,%s", "cmd2", p->saleNum, p->payMoney, payMode, p->payQr,  "@#$");
    pthread_mutex_unlock (&mutex);
}
