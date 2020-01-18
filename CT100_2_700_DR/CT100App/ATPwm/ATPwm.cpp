#include "ATPwm.h"

CPwm pwm;

int ATPwm_Init()
{
    return pwm.init();
}

int ATPwm_SetBackLight(int value)
{
    return pwm.SetBacklight(value);
}




