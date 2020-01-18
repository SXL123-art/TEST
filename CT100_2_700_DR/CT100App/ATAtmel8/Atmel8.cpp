#include "Atmel8.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include "../ATLib/SysApi.h"
#include  "../ATBeep/ATBeep.h"

void *theadAtmel8(void *p)
{
    static unsigned char nCheckSerialCnt = 0;
    CAtmSerial* pAtmel8 = (CAtmSerial*)p;

    while(1)
    {
        usleep(50*1000);
        char buf[1024] = {0};
        if (!pAtmel8->m_Atm8Serial)
        {
            break;
        }
        int nRecvLen = pAtmel8->m_Atm8Serial->read_port(buf, 1024);
        unsigned char nCheckCode = 0;
        if (nRecvLen > 0)
        {
            for(int i=1; i<nRecvLen-1; i++)
            {
                nCheckCode ^= buf[i];
            }
        }

        if (buf[0]==0x02 && buf[nRecvLen-2]==0x03 && buf[nRecvLen-1]==nCheckCode)
        {
            if (buf[3] == 0x01)
            {
                pAtmel8->m_bAc = buf[4];
                pAtmel8->m_bPoweroff = buf[5];
                pAtmel8->m_nBatteryVol =((BcdToHex(buf[6])*100) + BcdToHex(buf[7]));
            }
            nCheckSerialCnt = 0;
            pAtmel8->m_bCheckSerial = true;

            if (pAtmel8->m_bPoweroff)
            {
                ATBeepErr();
                while(1)
                {
                    pAtmel8->SystemToCpu(1, 7);
                }
            }
            else
            {
                pAtmel8->SystemToCpu(0, 7);
            }
        }
        else
        {
            if (++nCheckSerialCnt >= 50)
            {
                nCheckSerialCnt = 0;
                pAtmel8->m_bCheckSerial = false;
            }
        }
    }
}

CAtmSerial::CAtmSerial(void)
{
    m_bAc = false;
    m_bPoweroff = false;
    m_bCheckSerial = false;
    m_nBatteryVol = 0;
}

int CAtmSerial::Init(void)
{
    m_Atm8Serial = new CMySerial();

    if (!m_Atm8Serial)
    {
        return -1;
    }

    if(0 < m_Atm8Serial->open_port(SERIAL_ATMEL))
    {
        //????????
        m_Atm8Serial->setup_port(ATMEL_BAUD, 8, 'N', 1, NoFlowControl);
        printf("\nATM...............................[Y]\n");
    }
    else
    {
         printf("\nATM...............................[X]\n");
         return -2;
    }
    pthread_t idAtmel8;
    pthread_create(&idAtmel8, NULL, theadAtmel8, this);
}

CAtmSerial::~CAtmSerial(void)
{

}

int CAtmSerial::SystemToCpu(unsigned char Value, unsigned int Len)
{
    int nRet = 0;
    char temp[64] = {0};
    unsigned char nCheckCode = 0;
    if (!m_Atm8Serial)
            return -1;
    temp[0] = 0x02;
    temp[1] = 0x00;
    temp[2] = 0x03;
    temp[3] = 0x81;
    temp[4] = Value;
    temp[5] = 0x03;
    for (int i=0; i<Len-1; i++)
    {
        nCheckCode ^= temp[i];
    }
    temp[6] = nCheckCode;
    nRet = m_Atm8Serial->write_port(temp, Len);
    if (nRet < 0)
    {
        printf("Send ATM Device Cmd Fail...\n");
        return -2;
    }
    return 0;
}
