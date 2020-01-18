
#ifndef ATMEL8_H
#define ATMEL8_H

#include "../ATSerialDevice/CSerial.h"
#include  "../ATSerialDevice/typeSerial.h"

class CAtmSerial
{
public:
        CAtmSerial(void);

        ~CAtmSerial(void);

        int Init(void);
        int SystemToCpu(unsigned char Value, unsigned int Len);

public:
         CMySerial	*m_Atm8Serial;

         bool m_bAc;
         bool m_bPoweroff;
         bool m_bCheckSerial;
         unsigned int  m_nBatteryVol;
};


#endif
