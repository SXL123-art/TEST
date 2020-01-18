



/*
================================================================================
				 EAN条码字库（0 —9）及其编码表
================================================================================
*/

//  数字编码表，在不同的编码模式下，0-9采用不同的编码
const unsigned char EAN_EncodingTable[3][10]=
{
    {0X0D, 0X19, 0X13, 0X3D, 0X23, 0X31, 0X2F, 0X3B, 0X37, 0X0B},	//A
    {0x27, 0X33, 0X1B, 0X21, 0X1D, 0X39, 0X05, 0X11, 0X09, 0X17},	//B
    {0X72, 0X66, 0X6C, 0X42, 0X5C, 0X4E, 0X50, 0X44, 0X48, 0X74}
};	//C
//    0      1     2     3     4     5     6     7     8     9


//-------------------------------------------------
//  奇偶性参照表，第一个数制位决定左手边6位数字编码模
//  式，其中'O'表示“奇数性”，'E'表示“偶数性”
//  -------------------------------------------------
const unsigned char EAN_parityTable[10][6]=
{
    {'O', 'O', 'O', 'O', 'O', 'O'}, 	//0
    {'O', 'O', 'E', 'O', 'E', 'E'},		//1
    {'O', 'O', 'E', 'E', 'O', 'E'},		//2
    {'O', 'O', 'E', 'E', 'E', 'O'},		//3
    {'O', 'E', 'O', 'O', 'E', 'E'},		//4
    {'O', 'E', 'E', 'O', 'O', 'E'},		//5
    {'O', 'E', 'E', 'E', 'O', 'O'},		//6
    {'O', 'E', 'O', 'E', 'O', 'E'},		//7
    {'O', 'E', 'O', 'E', 'E', 'O'},		//8
    {'O', 'E', 'E', 'O', 'E', 'O'}
};	//９


//-------------------------------------------------
//  数字模板表，用来打印0-9可视化数字的打印模板，每个
//  数字打印模板都是7*7的矩形二维数组
//  -------------------------------------------------
const unsigned char EAN_numberTable[10][7] =
{
    {0x1C, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C},		//0
    {0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x1C},		//1
    {0x1C, 0x22, 0x02, 0x04, 0x08, 0x10, 0x3E},		//2
    {0x1C, 0x22, 0x02, 0x0E, 0x02, 0x22, 0x1C},		//3
    {0x02, 0x06, 0x0A, 0x12, 0x22, 0x3F, 0x02},		//4
    {0x3E, 0x20, 0x3C, 0x02, 0x02, 0x22, 0x1C},		//5
    {0x1C, 0x20, 0x20, 0x3C, 0x22, 0x22, 0x1C},		//6
    {0x3E, 0x02, 0x04, 0x08, 0x08, 0x08, 0x08},		//7
    {0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C},		//8
    {0x1C, 0x22, 0x22, 0x1E, 0x02, 0x02, 0x1C}
};	//9

/*
================================================================================
				 code128码编码表
================================================================================
*/
const unsigned short Code128Tab[107]=  //code128码编码表
{
    0x6cc,0x66c,0x666,0x498,0x48c,0x44c,0x4c8,0x4c4,0x464,0x648,	//0  - 9
    0x644,0x624,0x59c,0x4dc,0x4ce,0x5cc,0x4ec,0x4e6,0x672,0x65c,	//10 - 19
    0x64e,0x6e4,0x674,0x76e,0x74c,0x72c,0x726,0x764,0x734,0x732,	//20 - 29
    0x6d8,0x6c6,0x636,0x518,0x458,0x446,0x588,0x468,0x462,0x688,	//30 - 39
    0x628,0x622,0x5b8,0x58e,0x46e,0x5d8,0x5c6,0x476,0x776,0x68e,	//40 - 49
    0x62e,0x6e8,0x6e2,0x6ee,0x758,0x746,0x716,0x768,0x762,0x71a,	//50 - 59
    0x77a,0x642,0x78a,0x530,0x50c,0x4b0,0x486,0x42c,0x426,0x590,	//60 - 69
    0x584,0x4d0,0x4c2,0x434,0x432,0x612,0x650,0x7ba,0x614,0x47a,	//70 - 79
    0x53c,0x4bc,0x49e,0x5e4,0x4f4,0x4f2,0x7a4,0x794,0x792,0x6de,	//80 - 89
    0x6f6,0x7b6,0x578,0x51e,0x45e,0x5e8,0x5e2,0x7a8,0x7a2,0x5de,	//90 - 99
    0x5ee,0x75e,0x7ae,0x684,0x690,0x69c,0x18eb						//100-106
};















