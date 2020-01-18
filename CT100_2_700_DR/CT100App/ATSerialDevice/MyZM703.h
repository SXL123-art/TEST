#ifndef _MY_RFID_H
#define _MY_RFID_H

// ��λ�Զ�����Ѱ��52 
int Rfid_StartCard();
// ֹͣ�Զ�Ѱ��
int Rfid_StopCard();

/*
	M1���ӿ�
	M1����ΪS50��S70��
	S50��16��������0-15��������1K
	S70��32��������0-31��������2K
	ÿ������4�飬ÿ��16���ֽڣ���3Ϊ���ƿ飬����������A����ȡ���ơ�����B
	�������̣�
	1����λ�Զ�����Ѱ��52
	2����������/������д
	3��ͣ��
*/
// ��Կ��֤
int Rfid_KeyAuthentication_M1(unsigned char sectionNum, char *passwd); 
// ��������
int Rfid_ReadBlock_M1(unsigned char blockNum, char *readData);
// д������
int Rfid_WriteBlock_M1(unsigned char blockNum, char *writeData, unsigned int len);
// ��ʼ��Ǯ����blockNumǮ�����ڵĿ�ţ�money��ʼ�������ֽ���ǰ�����ֽ��ں�
int Rfid_WalletInit_M1(unsigned char blockNum, char *money);
// ��Ǯ��
int Rfid_ReadWallet_M1(unsigned char blockNum, char *money);
// Ǯ����ֵ/��ֵ
int Rfid_AddOrSubWallet_M1(bool addWallet, unsigned char blockNum, char *money);
// ���϶��飬��sectionNum������blockNum���ݿ�
int Rfid_ReadCompositeBlock_M1(unsigned char sectionNum, unsigned char blockNum, char *passwd, char *readData);
// ����д�飬дsectionNum������blockNum���ݿ�
int Rfid_WriteCompositeBlock_M1(unsigned char sectionNum, unsigned char blockNum, char *passwd, char *writeData, unsigned int len);
// ������������beginBlockNum���ݿ����endBlockNum���ݿ飬���ݿ�3�������������ݳ���
int Rfid_ReadSection_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *readData);
// д����������beginBlockNum���ݿ�д��endBlockNum���ݿ飬���ݿ�3��д
int Rfid_WriteSection_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *writeData, unsigned int len);
// ͣ��
int Rfid_StoringCard_M1(void);

/*
	CPU���ӿ�
	�������̣�
	1����λ�Զ�����Ѱ��52
	2��CPU��ת��
	3����COS����
	4��CPU��ͣ��
*/
// CPU��ת��
int Rfid_Enter_CPU(void); 
// COSָ��
int Rfid_Cos_CPU(unsigned char CosLen, unsigned char *pCos, char *pRecv);
// ���϶����������ļ� 
int Rfid_Read_CPU(void); 
// ����д���������ļ� 
int Rfid_Write_CPU(void);
// ͣ��
int Rfid_Storing_CPU(void);
// ��Կ��֤
int Rfid_KeyAuthentication_CPU(unsigned char sectionNum, unsigned char *passwd);
// ����
int Rfid_Clear_CPU(void);

/*
	PSAM���ӿ�
	CPU���ӿ�
	�������̣�
	1��PSAM����λ
	2����COS����
*/
// PSAM����λ
int Rfid_Reset_PSAM(void); 
// COSָ�ȡ8λ�����
int Rfid_Cos_PSAM(void);

#endif

