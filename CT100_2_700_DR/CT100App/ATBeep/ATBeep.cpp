#include "ATBeep.h"

#define 	BEEP	1

int ATBeep()
{
#if BEEP
	beep_on();
	usleep(1000*150);
	beep_off();
#endif
}

int ATBeepErr()
{
#if BEEP
	beep_on();
	usleep(1000*100);
	beep_off();
	usleep(1000*100);
	beep_on();
	usleep(1000*100);
	beep_off();
	usleep(1000*100);
	beep_on();
	usleep(1000*100);
	beep_off();
#endif
}

int ATBeepInit()
{
#if BEEP
	beep_init();

	ATBeep();
#endif
}


