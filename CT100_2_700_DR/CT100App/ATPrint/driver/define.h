
#define BUF_H16                 16
#define BUF_H32                 32
#define PRT_TYPE_BOLD           0x01
#define PRT_TYPE_UNDERLINE      0x02
#define PRT_TYPE_STROKE         0x04
#define PRT_TYPE_DOUBLE_STROKE  0x08

//----------------------------------------
#define   S0                0x00                                     // 打印机的初始状态
#define   ESC_CMD           0x01                                     // 接收ESC命令的状态
#define   ESC_W             0x02
#define   ESC_U             0x03
#define   ESC_V             0x04
#define   ESC_m             0x05
#define   ESC_E             0x06
#define   ESC_p             0x07
#define   ESC_p_m           0x08
#define   ESC_1             0x09
#define   ESC_K             0x0a
#define   ESC_K_N1          0x0b
#define   ESC_K_N1_N2       0x0c
#define   ESC_T             0x0d

#define   FS_CMD            0x10                                     // 接收FS命令的状态
#define   FS_COND           0x11                                     // 控制打印浓度命令状态
#define   FS_SERIAL         0x12                                     // Change the serial speed state
#define   FS_TEST           0x13
#define   SI_CMD            0x20
#define   DBCS              0x40				//准备打印中文标志

#define   STATE_DOWNLOAD    0x60

#define   NUL             0x00
#define   HT              0x09
#define   LF              0x0a
#define   VT              0x0b
#define   FF              0x0c
#define   CR              0x0d
#define   SO              0x0e
#define   SI              0x0f
#define   DC4             0x14
//#define   CAN             0x18
#define   ESC             0x1b
#define   DEBUG           0x02
#define   ACK             0x06
#define   FS              0x1c
#define   NAK             0x15

#define   PRT_MAX_WIDTH     384									//一行最多384点
//---------------------------------

#define SPEED85 183
#define SPEED80 195
#define SPEED75	206			//206 --> 75.8mm/s
#define SPEED70	223			//
#define SPEED65	240			//
#define SPEED50	312			//
#define SPEED40	390			//

#define FontNO    0
#define FontBOLD  1
#define FontWIDE  2
#define FontHIGH  3
#define FontBIG   4
#define FontUNDERLINE 5
#define FontSTRIKETHROUGH 6

