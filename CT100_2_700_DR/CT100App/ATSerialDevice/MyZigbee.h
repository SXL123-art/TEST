

int ResetZigbee(void);
int BeginZigbeeConfig(void);
int ReadZigbeeConfig(void);
int SaveZigbeeConfig(int nTypeNet, int nChannel);
int SendAndRecvZigbeeData(char *sendData, int nsendLen, char *recvData, int nrecvLen);
