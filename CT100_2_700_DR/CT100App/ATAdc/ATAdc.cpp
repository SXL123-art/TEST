// loaddll.cpp : Defines the entry point for the DLL application.
//
#include "ATAdc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Ad.h"
#define 	ADSTABE_INIT_TIMES	25
#define 	MINADINTERVAL		150	//这个值最小值
CAdc adc;
adc_param	m_param;
adc_ret		m_adInfo;
void* theadAdMaster(void* p)
{
    //得到重量
    int i;
    for(i=0;i<80;i++)//200
    {
        if(i>ADSTABE_INIT_TIMES){
                usleep((MINADINTERVAL)*1000+40*(i%5));
        }else{
                usleep((MINADINTERVAL+10)*1000);//这个值经过多次试验，最佳
        }
        ATAd_getKg(&(m_adInfo));
        printf("---------------------------%d  wait for zero! %d %d %ld\n",i,m_adInfo.stabok,m_param.full_arrange*1000/10,m_adInfo.kg);
        if(m_adInfo.stabok == 1 && (m_param.full_arrange*1000/10 >= m_adInfo.kg))
        {
            ATAd_setZero();
            printf("---------------------------wait for CAdMaster stabok!\n");
            return NULL;
        }
    }
}
int ATAd_initAdc_param()
{
    //下面加载电子秤标定信息
    memset((void *)&m_param,0,sizeof(adc_param));
    QSqlQuery query;
    if(ATdb_Select_ReturnList_QSqlQuery_By_SQL_Statements("select *from TABLE_CALIBRATION where id=1;",&query)  > 0)
    {
        query.next();
        bool ok;
        m_param.full_arrange = query.record().value(TABLE_CALIBRATION_FULLARRANGE).toString().toInt(&ok,10);
        m_param.cal_arrange = query.record().value(TABLE_CALIBRATION_CALARRANGE).toString().toInt(&ok,10);
        m_param.e = query.record().value(TABLE_CALIBRATION_E).toString().toInt(&ok,10);
        m_param.e1 = query.record().value(TABLE_CALIBRATION_E1).toString().toInt(&ok,10);
        m_param.gl = query.record().value(TABLE_CALIBRATION_LOCALRAVITY).toString().toInt(&ok,10);
        m_param.gd = query.record().value(TABLE_CALIBRATION_DEGRAVITY).toString().toInt(&ok,10);
        m_param.unload = query.record().value(TABLE_CALIBRATION_UNLOAD).toString().toInt(&ok,10);
        m_param.load = query.record().value(TABLE_CALIBRATION_LOAD).toString().toInt(&ok,10);
        m_param.is_dual = query.record().value(TABLE_CALIBRATION_ISDUAL).toString().toInt(&ok,10);

    }
   ATAd_initAdc(m_param);
      pthread_t idAdMaster;
      int pthread = pthread_create(&idAdMaster, NULL, theadAdMaster, NULL);
      if (pthread)
      {
          errno = pthread;
          perror("create theadAdMaster thread");
      }
      pthread = pthread_detach(idAdMaster);
      if (pthread) {
          errno = pthread;
          perror("detach theadAdMaster thread");
      }
}

int ATAd_initAdc(adc_param  ap)
{
    adc.load_param(ap);
    return 0;
}

int ATAd_getKg(adc_ret * ret)
{
    if(0 == adc.getkg(0))//fail
    {
        ret->kg = adc.kg;
        ret->kgtara = adc.kgtara / adc.MAX_DIV;
        ret->overok = adc.overok;
        ret->stabok = adc.stabok;
        ret->taraok = adc.taraok;
        ret->zerook = adc.zerook;
        if(ret->kg<0)
        {
            ret->zerook = 0;
        }
        return 1;
    }

    ret->kg = adc.kg;
    ret->kgtara = adc.kgtara / adc.MAX_DIV;
    ret->overok = adc.overok;
    ret->stabok = adc.stabok;
    ret->taraok = adc.taraok;
    ret->zerook = adc.zerook;
    if(ret->kg<0)
    {
        ret->zerook = 0;
    }
    return 0;
}

adc_param ATAd_getParam()
{
    return m_param;
}

unsigned long ATAd_getInternalCode()
{
    return adc.getInternalCode();
}

int ATAd_setZero()//adc_ret * ret
{
    int iOK;
    usleep(kilo*300);
    iOK = adc.getkg(1);
    usleep(kilo*300);
    return iOK;
}

int ATAd_setTara()//adc_ret * ret
{
    int iOK;
    usleep(kilo*300);
    iOK = adc.getkg(2);
    usleep(kilo*300);
    return iOK;
}

