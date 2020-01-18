#include "CSerial.h"
#include "MyZigbee.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "typeSerial.h"



#define PMU_IOC_MAGIC	'y'
#define PMU_ZIGBEE_HIGH 	_IOR(PMU_IOC_MAGIC, 15, unsigned int)
#define PMU_ZIGBEE_LOW	 	_IOR(PMU_IOC_MAGIC, 16, unsigned int)



typedef struct _tagZIGBEEINFOORG
{
	char address[2];
	char id;
	char typeNet;
	char typeNode;
	char sendMode;
	char baud;
	char jiaoyan;		//校验位
	char data;		//数据位
	char datamode;
	char delay;
	char xindao;
	char gonglv;
	char addoption;
}stZigbeeInfoOrg,*pstZigbeeInfoOrg;

//方便操作数据格式
typedef struct _tagZIGBEEINFO
{
	char address[3];
	char id[2];
	char typeNet[2];
	char typeNode[2];
	char sendMode[2];
	char baud[2];
	char jiaoyan[2];		//校验位
	char data[2];		//数据位
	char datamode[2];
	char delay[2];
	char xindao[2];
	char gonglv[2];
	char addoption[2];
}stZigbeeInfo,*pstZigbeeInfo;


int ZigbeeSendAndRecv(int nSpeed, char *sendData,  int nDataLen, char *recvData, int nrecvLen)
{
	CSerial slaveSerial;
	int nRet = slaveSerial.Init(ZIGBEE_PORT, nSpeed, 8, 'N', 1, NoFlowControl);
	if (nRet < 0)
	{
		printf("slaveSerial open faile\n");
		return -1;
	}

	nrecvLen = slaveSerial.SendAndRecvData(sendData, nDataLen, recvData, 3);
	if (nrecvLen < 0)
	{
		printf("SendAndRecvData Fail...\n");
		return -2;
	}
	return 0;
}

int ResetZigbee()
{
	char command[2];
	command[0]=0x23;
	command[1]=0x23;

	int nLen = 0;
	char RecvBuffer[64] = {0};
	int nZigbeeRet = ZigbeeSendAndRecv(ZIGBEE_CONFIG_BAUD, command, 2, RecvBuffer, nLen);
	if (nZigbeeRet < 0)
	{
		printf("-------- ZigbeeSendAndRecv fail--------\n");
		return -1;
	}
}
int BeginZigbeeConfig()
{
        int fdPMU = open("/dev/ct200gc_pmu", O_RDWR);
	if (fdPMU < 0)
	{
		perror("can not open device ct100gc_pmu!");
		exit(1);
	}
	else
	{
		ioctl(fdPMU, PMU_ZIGBEE_LOW, NULL);	
		printf("CFG LOW\n");
		sleep(3);	
		ioctl(fdPMU, PMU_ZIGBEE_HIGH, NULL);	
		printf("CFG LOW\n");
	}	
}

int ReadZigbeeConfig()
{
	char command[2];
	command[0] = 0x23;
	command[1] = 0xA0;

	int nLen = 0;
	char RecvBuffer[64] = {0};
	int nZigbeeRet = ZigbeeSendAndRecv(ZIGBEE_CONFIG_BAUD, command, 2, RecvBuffer, nLen);
	if (nZigbeeRet < 0)
	{
		printf("-------- ZigbeeSendAndRecv fail--------\n");
		return -1;
	}
	stZigbeeInfoOrg m_infoOrg;
	memcpy((void*)&m_infoOrg, RecvBuffer, 14);
}
int SaveZigbeeConfig(int nTypeNet, int nChannel)
{
	stZigbeeInfoOrg m_infoOrg;
	//m_infoOrg.id = 0x0F;
	m_infoOrg.id = nTypeNet;
	m_infoOrg.typeNet = 0x01;
	m_infoOrg.typeNode = 0x01;
	m_infoOrg.sendMode = 0x01;
	m_infoOrg.baud = 0x04;
	m_infoOrg.jiaoyan = 0x01;
	m_infoOrg.data = 0x01;
	m_infoOrg.datamode = 0x02;
	m_infoOrg.delay = 0x0A;
	//m_infoOrg.xindao = 0x0F;
	m_infoOrg.xindao = nChannel;
	m_infoOrg.gonglv = 0x00;
	m_infoOrg.addoption = 0x01;
	
	char command[16];
	command[0]=0x23;
	command[1]=0xFE;
	memcpy(command+2, (void*)&m_infoOrg, 14);

	int nLen = 0;
	char RecvBuffer[64] = {0};
	int nZigbeeRet = ZigbeeSendAndRecv(ZIGBEE_CONFIG_BAUD, command, 2, RecvBuffer, nLen);
	if (nZigbeeRet < 0)
	{
		printf("-------- ZigbeeSendAndRecv fail--------\n");
		return -1;
	}
}
int SendAndRecvZigbeeData(char *sendData, int nsendLen, char *recvData, int nrecvLen)
{
	return ZigbeeSendAndRecv(ZIGBEE_BAUD, sendData, nsendLen, recvData, nrecvLen);
}

