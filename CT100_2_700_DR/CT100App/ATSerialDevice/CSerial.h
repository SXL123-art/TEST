#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "driver_serial.h"

class CSerial
{
public:
	CSerial(void);
	~CSerial(void);

public:
	int Init(int nPort, int nSpeed, int nDataBits, char Parity, int nStopBits, FlowControl fc);
	
	int SendAndRecvData(char *sendData, int nsendLen, char *recvData, int recvLen);
	
	int RecvData(char *recvData, int recvTimeout);

	void setStopRecv(bool bStop);
public:
	bool b_SerialOpen;
	bool b_StopRecv;
	CMySerial *m_serial;
private:

};

#endif

