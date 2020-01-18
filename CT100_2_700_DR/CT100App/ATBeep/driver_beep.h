///*****************************************************************************
/*
|*  Copyright       :   (c) 2011 CAS
|*  Filename        :   driver_beep.h
|*  Version         :   0.1
|*  Programmer(s)   :   Tang Qian Kun(TQK)
|*  Created         :   2011/4/6
|*  Modified        :
|*  Description     :   CT100 beep function header
*/
//****************************************************************************/
#include "../public/globals.h"
#include <unistd.h>

 void beep_off(void);
 void beep_on(void);
 void beep_init(void);
 void open_dev_buzzer(void);
 void close_dev_buzzer(void);
 
