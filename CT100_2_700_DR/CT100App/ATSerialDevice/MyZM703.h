#ifndef _MY_RFID_H
#define _MY_RFID_H

// 复位自动复合寻卡52 
int Rfid_StartCard();
// 停止自动寻卡
int Rfid_StopCard();

/*
	M1卡接口
	M1卡分为S50和S70卡
	S50：16个扇区，0-15扇区，共1K
	S70：32个扇区，0-31扇区，共2K
	每个扇区4块，每块16个字节，块3为控制块，包括了密码A、存取控制、密码B
	操作流程：
	1、复位自动复合寻卡52
	2、多扇区读/多扇区写
	3、停卡
*/
// 秘钥认证
int Rfid_KeyAuthentication_M1(unsigned char sectionNum, char *passwd); 
// 读块数据
int Rfid_ReadBlock_M1(unsigned char blockNum, char *readData);
// 写块数据
int Rfid_WriteBlock_M1(unsigned char blockNum, char *writeData, unsigned int len);
// 初始化钱包，blockNum钱包所在的块号，money初始化金额，低字节在前，高字节在后
int Rfid_WalletInit_M1(unsigned char blockNum, char *money);
// 读钱包
int Rfid_ReadWallet_M1(unsigned char blockNum, char *money);
// 钱包充值/减值
int Rfid_AddOrSubWallet_M1(bool addWallet, unsigned char blockNum, char *money);
// 复合读块，读sectionNum扇区的blockNum数据块
int Rfid_ReadCompositeBlock_M1(unsigned char sectionNum, unsigned char blockNum, char *passwd, char *readData);
// 复合写块，写sectionNum扇区的blockNum数据块
int Rfid_WriteCompositeBlock_M1(unsigned char sectionNum, unsigned char blockNum, char *passwd, char *writeData, unsigned int len);
// 读多扇区，从beginBlockNum数据块读到endBlockNum数据块，数据块3不读，返回数据长度
int Rfid_ReadSection_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *readData);
// 写多扇区，从beginBlockNum数据块写到endBlockNum数据块，数据块3不写
int Rfid_WriteSection_M1(unsigned char beginBlockNum, unsigned char endBlockNum, char *passwd, char *writeData, unsigned int len);
// 停卡
int Rfid_StoringCard_M1(void);

/*
	CPU卡接口
	操作流程：
	1、复位自动复合寻卡52
	2、CPU卡转入
	3、发COS命令
	4、CPU卡停卡
*/
// CPU卡转入
int Rfid_Enter_CPU(void); 
// COS指令
int Rfid_Cos_CPU(unsigned char CosLen, unsigned char *pCos, char *pRecv);
// 复合读长二进制文件 
int Rfid_Read_CPU(void); 
// 复合写长二进制文件 
int Rfid_Write_CPU(void);
// 停卡
int Rfid_Storing_CPU(void);
// 秘钥认证
int Rfid_KeyAuthentication_CPU(unsigned char sectionNum, unsigned char *passwd);
// 擦除
int Rfid_Clear_CPU(void);

/*
	PSAM卡接口
	CPU卡接口
	操作流程：
	1、PSAM卡复位
	2、发COS命令
*/
// PSAM卡复位
int Rfid_Reset_PSAM(void); 
// COS指令，取8位随机数
int Rfid_Cos_PSAM(void);

#endif

