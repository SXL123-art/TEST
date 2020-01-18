#ifndef _DRIVER_PwM_H
#define _DRIVER_PwM_H

#include <unistd.h>

class CPwm
{	
public:
	CPwm();
	~CPwm();

	int init();
	int SetBacklight(int value);

private:
	int	fd;
};



#endif