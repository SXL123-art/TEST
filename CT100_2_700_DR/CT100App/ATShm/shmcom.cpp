#include "shmcom.h"
#include <errno.h>
#include "ATAdc/ATAdc.h"
#include "unit/shareInfo.h"

shmCom::shmCom(decimal	*pDecimal)
{
    m_deci = pDecimal;
    m_bReady = false;
    m_bSetZero = false;
}

shmCom::~shmCom(void)
{

}

void* PaintGuiQt(void* p)
{
    stTabSaleUi stSaleInfo;
    adc_ret		m_adInfo;
    memset(&stSaleInfo, 0x00, sizeof(stTabSaleUi));
    sprintf(stSaleInfo.ver,"%s",SOFTWARE_VER);
    shmCom* pCGuiQt = (shmCom*)p;
    for (int i=0; i<10; i++)
    {
        ATAd_getKg(&(m_adInfo));//这样做的原因是，循环自检后没有置零就报错，防止开机压重量或者开机一直未稳定状态,后面自动置零了
        printf("---------------------------%d  wait for zero! %d %d %d %ld\n",i,pCGuiQt->m_bSetZero,m_adInfo.stabok,m_param.full_arrange*1000/10,m_adInfo.kg);
        if(pCGuiQt->m_bSetZero==false && m_adInfo.stabok == 1 && (m_param.full_arrange*1000/10 >= abs(m_adInfo.kg)))
        {
            ATAd_setZero();
            printf("---------------------------wait for CAdMaster stabok!\n");
            pCGuiQt->m_bSetZero = true ;
            //sprintf(stSaleInfo.message,"%s","提示:重量自动归零成功");
        }
        // 显示0-9
        // 显示6位数字
        memset(stSaleInfo.weight,0x30+i, 6);
        memset(stSaleInfo.unitprice,0x30+i, 6);
        memset(stSaleInfo.totalprice,0x30+i,6);
        memset(stSaleInfo.tare,0x30+i, 6);
        memset(stSaleInfo.plu,0x30+i, 6);
        printf("---------------------------wait for CAdMaster stabok %d\n",i);
        stShowQT* pQT = (stShowQT*)(pCGuiQt->shmAddress);
        sem_wait(pCGuiQt->semw);
        pQT->type = enQt_SaleQT;
        pQT->update = true;
        pQT->saleUi= stSaleInfo;
        sem_post(pCGuiQt->semr);
        usleep(1000*200);
    }
    if(pCGuiQt->m_bSetZero==false)
    {
        sprintf(stSaleInfo.message,"%s","报警:重量未置零,请重新开机检测");
        stShowQT* pQT = (stShowQT*)(pCGuiQt->shmAddress);
        sem_wait(pCGuiQt->semw);
        pQT->type = enQt_SaleQT;
        pQT->update = true;
        pQT->saleUi= stSaleInfo;
        sem_post(pCGuiQt->semr);
    }
    else
    {
        sprintf(stSaleInfo.message,"%s","提示:重量自动归零成功");
        stShowQT* pQT = (stShowQT*)(pCGuiQt->shmAddress);
        sem_wait(pCGuiQt->semw);
        pQT->type = enQt_SaleQT;
        pQT->update = true;
        pQT->saleUi= stSaleInfo;
        sem_post(pCGuiQt->semr);
    }
    pCGuiQt->m_bReady = true;
}
void shmCom::initShm()
{
    // 创建并初始化有名信号灯 读
    semr = sem_open("mysem_r", O_CREAT | O_RDWR , 0666, 0);
    if (semr == SEM_FAILED)
    {
        perror("init semr:");
        exit(-1);
    }
    // 创建并初始化有名信号灯 写
    semw = sem_open("mysem_w", O_CREAT | O_RDWR, 0666, 1);
    if (semw == SEM_FAILED)
    {
        perror("init semr:");
        sem_close(semr);
        exit(-1);
    }
    int shmid_def;

    // 创建共享内存
    shmid_def = shmget((key_t)1236, sizeof(char)*SHAREMEM_LEN, 0666|IPC_CREAT);
    printf("App  shmCom  shmid_def:  %d\n",shmid_def);
    if (shmid_def == -1)
    {
        fprintf(stderr, "App shmget failed\n");
        exit(EXIT_FAILURE);
    }

    if ((shmAddress = (char *)shmat(shmid_def, NULL, 0)) == (char *)(-1))
    {
        perror("App int shmat:");
        sem_close(semr);
        sem_close(semw);
        exit(-1);
    }
    printf("[CT100G_APP]-----------------------------Shared memory [shmid]:%d [size]:%d\n",shmid_def,SHAREMEM_LEN);
}

void shmCom::init()
{
    initShm();

//    pthread_t idPaintGuiQT;
//    int pthread = pthread_create(&idPaintGuiQT, NULL, PaintGuiQt, this);
//    if (pthread)
//    {
//        errno = pthread;
//        perror("create PaintGuiQt thread");
//    }
//    pthread = pthread_detach(idPaintGuiQT);
//    if (pthread) {
//        errno = pthread;
//        perror("detach PaintGuiQt thread");
//    }
}
void shmCom::SetMaxWeight(unsigned int maxWeight)
{
    printf("m_ShareSystemInfo.MaxWeight:%d\n",maxWeight);
    m_nMaxWeight = maxWeight;
    m_bReady = true;
}

//皮重，重量
void shmCom::addDotLogin(stTabLoginUi* p)
{ 
    long value = 0;
    value = atol(p->tare);
    if(m_deci->dec_pt_TARE == 3)
        sprintf(p->tare,"%0d.%03d",value/1000,abs(value)%1000);
    else if(m_deci->dec_pt_TARE == 2)
        sprintf(p->tare,"%0d.%02d",value/100,abs(value)%100);

     value = atol(p->weight);
   if(value > m_nMaxWeight)
   {
       sprintf(p->weight,"%s","---------");
   }
   else
   {
       if(m_deci->dec_pt_WEIGHT == 3)
           sprintf(p->weight,"%d.%03d",value/1000,abs(value)%1000);
       else if(m_deci->dec_pt_WEIGHT == 2)
           sprintf(p->weight,"%d.%02d",value/100,abs(value)%100);
       //printf("---------------------------weight->:%d\n", value);
   }
}

//皮重，重量，单价，总价，小数点
void shmCom::addDotSale(stTabSaleUi* p)
{
    long value = 0;
    value = atol(p->tare);
    if (value < 0)
    {
        if(m_deci->dec_pt_TARE == 3)
            sprintf(p->tare,"-%0d.%03d",abs(value)/1000,abs(value)%1000);
        else if(m_deci->dec_pt_TARE == 2)
            sprintf(p->tare,"-%0d.%02d",abs(value)/100,abs(value)%100);
    }
    else
    {
        if(m_deci->dec_pt_TARE == 3)
            sprintf(p->tare,"%0d.%03d",abs(value)/1000,abs(value)%1000);
        else if(m_deci->dec_pt_TARE == 2)
            sprintf(p->tare,"%0d.%02d",abs(value)/100,abs(value)%100);
    }

    if(p->saleMode != 1)
    {
        value = atol(p->weight);
        if(value > m_nMaxWeight)
        {
            sprintf(p->weight,"%s","---------");
        }
        else
        {
            if (value < 0)
            {
                if(m_deci->dec_pt_WEIGHT == 3)
                    sprintf(p->weight,"-%d.%03d",abs(value)/1000,abs(value)%1000);
                else if(m_deci->dec_pt_WEIGHT == 2)
                    sprintf(p->weight,"-%d.%02d",abs(value)/100,abs(value)%100);
            }
            else
            {
                if(m_deci->dec_pt_WEIGHT == 3)
                    sprintf(p->weight,"%d.%03d",abs(value)/1000,abs(value)%1000);
                else if(m_deci->dec_pt_WEIGHT == 2)
                    sprintf(p->weight,"%d.%02d",abs(value)/100,abs(value)%100);
            }
        }
    }

    value = atol(p->unitprice);
    if(m_deci->dec_pt_UNIT == 2)
        sprintf(p->unitprice,"%d.%02d",value/100,abs(value)%100);
    else
        printf("---------------------------unitprice->dec_pt_UNIT != 2:%d\n", value);

    value = atol(p->totalprice);
    if(value >= 999999)
    {
        sprintf(p->totalprice,"%s","---------");
    }
    else
    {
        if(m_deci->dec_pt_TOTAL == 2)
            sprintf(p->totalprice,"%d.%02d",value/100,abs(value)%100);
        else
            printf("---------------------------unitprice->dec_pt_UNIT != 2:%d\n", value);
    }
}
void shmCom::show(char *p,enQtShowType Type)
{
    stShowQT* pQT = (stShowQT*)shmAddress;
    switch(Type)
    {
        case enQt_SaleQT:
            shmTabSaleUi = *((stTabSaleUi*)p);
            addDotSale(&shmTabSaleUi);
            sem_wait(semw);
            pQT->saleUi = shmTabSaleUi;
            pQT->type = enQt_SaleQT;
            pQT->update = true;
            //printf("------------------enQt_SaleQT\n");
            sem_post(semr);
            break;
        case enQt_LoginQT:
            shmTabLoginUi = *((stTabLoginUi*)p);
            addDotLogin(&shmTabLoginUi);
            sem_wait(semw);
            pQT->loginUi = shmTabLoginUi;
            pQT->type = enQt_LoginQT;
            pQT->update = true;
            sem_post(semr);
            break;
        case enQt_PayQT:
            sem_wait(semw);
            pQT->payUi = *((stTabPayUi*)p);
            pQT->type = enQt_PayQT;
            pQT->update = true;
            //printf("------------------enQt_PayQT\n");
            sem_post(semr);
            break;
        case enQt_MenuQT:
            sem_wait(semw);
            pQT->menuUi= *((stTabMenuUi*)p);
            pQT->type = enQt_MenuQT;
            pQT->update = true;
            sem_post(semr);
            break;
        case enQt_CalQt:
            sem_wait(semw);
            pQT->CalibrationUi= *((stTabCalibrationUi*)p);
            pQT->type = enQt_CalQt;
            pQT->update = true;
            sem_post(semr);
            break;
        case enQt_MsgQT:
            sem_wait(semw);
            pQT->msgUi= *((stMessageQT*)p);
            pQT->type = enQt_MsgQT;
            pQT->update = true;
            sem_post(semr);
            break;
        case enQt_InputQT:
            sem_wait(semw);
            pQT->inputUi= *((stInputQT*)p);
            pQT->type = enQt_InputQT;
            pQT->update = true;
            sem_post(semr);
            break;  
        case enQt_AddSaleQT:
            sem_wait(semw);
            pQT->addSaleUi= *((stAddSaleQT*)p);
            pQT->type = enQt_AddSaleQT;
            pQT->update = true;
            sem_post(semr);
            break;
        case enQt_ReportQt:
            sem_wait(semw);
            pQT->ReportUi= *((stReportQT*)p);
            pQT->type = enQt_ReportQt;
            pQT->update = true;
            sem_post(semr);
            break;
        case enQt_GoodsKeyQT:
            sem_wait(semw);
            pQT->goodsKeyUi= *((stGoodsKeyQT*)p);
            pQT->type = enQt_GoodsKeyQT;
            pQT->update = true;
            sem_post(semr);
            break;
        default:
            break;
    }
}
