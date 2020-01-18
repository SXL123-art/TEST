

#ifndef MYBLUETOOTH_H_
#define MYBLUETOOTH_H_


int Bluetooth_SendAndRecvData( char *sendData,  int nDataLen, char *recvData, int nrecvLen);
int Bluetooth_Reset();
int Bluetooth_GetMAC(char *MAC);
int Bluetooth_SetBroadcastcycle(int ms);
int Bluetooth_SetLinkInterval(int LinkTimeMS);
int Bluetooth_SetName(char *name);
int Bluetooth_SetBaudRate(int nSpeed);


#endif
