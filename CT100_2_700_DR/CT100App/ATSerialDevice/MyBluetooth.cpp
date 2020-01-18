#include "MyBluetooth.h"
#include "CSerial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typeSerial.h"

int Bluetooth_SendAndRecvData( char *sendData,  int nDataLen, char *recvData, int nrecvLen)
{
	CSerial slaveSerial;
	int nRet = slaveSerial.Init(BLUETOOTH_PORT, BLUETOOTH_BAUD, 8, 'N', 1, NoFlowControl);
	if (nRet < 0)
	{
		printf("slaveSerial open faile\n");
		return -1;
	}
	nrecvLen = slaveSerial.SendAndRecvData(sendData, nDataLen, recvData, 10);
	if (nrecvLen < 0)
	{
		printf("SendAndRecvData Fail...\n");
		return -2;
	}
	return 0;
}
int Bluetooth_Reset()
{
	char Reset[32] = "TTM:RST-SYSTEMRESET" ;
	char Return[32] = {0};
	int nRecvLen = 0;
	Bluetooth_SendAndRecvData(Reset,strlen(Reset),Return,nRecvLen);
	if(NULL == strstr(Return,"TTM:OK"))
	{
		printf("Bluetooth_Reset  success\n");
		return 0;
	}
	else
	{
		printf("Bluetooth_Reset fail\n");
		return -1;
	}	
}

int Bluetooth_GetMAC(char *MAC)
{
	char MACAT[12] = "TTM:MAC-?" ;
	char readMACAT[32] = {0};
	int nRecvLen = 0;
	Bluetooth_SendAndRecvData(MACAT,strlen(MACAT),readMACAT,nRecvLen);
	if(NULL == strstr(readMACAT,"TTM:OK"))
	{
		printf("Bluetooth_GetMAC  success\n");
		//解析得到MAC，并复制到MAC
		return 0;
	}
	else
	{
		printf("Bluetooth_Reset fail\n");
		return -1;
	}
}

/*
向串口 RX 输入以下字串， 设置模块的广播周期， T = X * 100ms
"TTM:ADP-(X)" ,X = "2","5","10","15","20","25","30","40","50" 之 一
*/
int Bluetooth_SetBroadcastcycle(int ms)
{
	int Broadcastcycle[] ={2,5,10,15,20,25,30,40,50};
	unsigned char len,i,flag = 0;
	
	len = sizeof(Broadcastcycle) / sizeof(int);
	for(i = 0; i< len;i++)
	{
		if(ms == Broadcastcycle[i])
		{
			flag++;
			char Broadcast[32] = {0};
			char trun[32] ={0};
			int nRecvLen = 0;
			sprintf(Broadcast,"TTM:ADP-(%d)",ms);
			Bluetooth_SendAndRecvData(Broadcast,strlen(Broadcast),trun,nRecvLen);
			
			if(NULL == strstr(trun,"TTM:OK"))
			{
				 printf("write Broadcastcycle success\n");
				 return 0;
			}
			else
			{
				 printf("write Broadcastcycle fail\n");
				 return -1;
			}
			
		}
	}
	if(!flag) printf("write Broadcastcycle error，ex :2,5,10,15 ... \n");
}

//设置蓝牙连接间隔
int Bluetooth_SetLinkInterval(int LinkTimeMS)
{
	int len,flag = 0;
	int link_time[] = {20,50,100,200,300,400,500,1000,1500,2000};
	len = sizeof(link_time) / sizeof(int);
	for(int i = 0; i<len ; i++)
	{
		if(LinkTimeMS == link_time[i])
		{
			flag++;
			char readtime[16] ={0};
			char writeTime[16] = {0};
			int nRecvLen = 0;
			sprintf(writeTime,"%s%dms","TTM:CIT-",LinkTimeMS);
			Bluetooth_SendAndRecvData(writeTime,strlen(writeTime),readtime,nRecvLen);
			
			if(NULL == strstr(readtime,"TTM:OK"))
			{
				 printf("write LinkTimeMS success\n");
				 return 0;
			}
			else
			{
				printf("write LinkTimeMS fail\n");
				return -1;
			}
			
		}
	}
	if(!flag) 
		printf("write LinkTimeMS error，ex :20 50 100 ... \n");

}

//蓝牙重命名
int Bluetooth_SetName(char *name)
{
	char readname[32] ={0};
	char writeName[32] = {0};
	sprintf(writeName,"%s%s","TTM:REN-",name);
	int nRecvLen = 0;
	Bluetooth_SendAndRecvData(writeName,strlen(writeName),readname,nRecvLen);
	if(NULL == strstr(readname,"TTM:OK"))
	{
		 printf("write Bluetooth name success\n");
		 return 0;
	}
	else
	{
		 printf("write Bluetooth  name fail\n");
		 return -1;
	}
}

int Bluetooth_SetBaudRate(int nSpeed)
{
	int name_arr[] = {115200, 9600, 38400, 19200, 4800};
	int len = sizeof(name_arr) / sizeof(int);
	int i;
	unsigned char flag = 0;
	for(i = 0; i<len ; i++)
	{
		if (nSpeed == name_arr[i]){
			flag++;
			char readspeed[32] ={0};
			char writeSpeed[32] = {0};
			sprintf(writeSpeed,"%s%d","TTM:BPS-",nSpeed);
			int nRecvLen = 0;
			Bluetooth_SendAndRecvData(writeSpeed,strlen(writeSpeed),readspeed,nRecvLen);
			printf("read speed:%s\n",readspeed);
			if(NULL == strstr(readspeed,"TTM:OK"))
			{
				 printf("write speed success\n");
				 return 0;
			}
			else
			{
				printf("write speed fail\n");
				return -1;
			}
		}
			
	}
	if(!flag) 
		printf("write speed error，ex :4800 9600 ... \n");
}