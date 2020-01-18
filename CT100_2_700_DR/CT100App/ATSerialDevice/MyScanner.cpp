#include "CSerial.h"
#include "MyScanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "typeSerial.h"


int ScannerRecv(char *recvData, int nrecvLen)
{
	CSerial slaveSerial;
	int nRet = slaveSerial.Init(SCANNER_PORT, SCANNER_BAUD, 8, 'N', 1, NoFlowControl);
	if (nRet < 0)
	{
		printf("slaveSerial open faile\n");
		return -1;
	}

	nrecvLen = slaveSerial.RecvData(recvData, 15);
	if (nrecvLen < 0)
	{
		printf("SendAndRecvData Fail...\n");
		return -2;
	}
	return 0;
}

int RecvScannerData(char *recvData, int nrecvLen)
{
	return ScannerRecv(recvData, nrecvLen);
}

