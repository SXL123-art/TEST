#ifndef TYPEKEY_H
#define TYPEKEY_H

#include "globals.h"
#include "../../Public/typeTabUi.h"

/*Layout of the key
----------------------------------------------------------------
| 01 | 02 | 03 | 04 | 05 |            | 31 | 32 | 33 | 34 | 35 |
----------------------------------------------------------------
| 06 | 07 | 08 | 09 | 10 |            | 36 | 37 | 38 | 39 | 40 |
----------------------------------------------------------------
| 11 | 12 | 13 | 14 | 15 |            | 57 | 58 | 59 | 41 | 42 |
----------------------------------------------------------------
| 16 | 17 | 18 | 19 | 20 |            | 54 | 55 | 56 | 43 | 44 |
----------------------------------------------------------------
| 21 | 22 | 23 | 24 | 25 |            | 51 | 52 | 53 | 45 | 46 |
----------------------------------------------------------------
| 26 | 27 | 28 | 29 | 30 |            | 47 | 50 | 48 |   49    |
----------------------------------------------------------------
*/
//TOTAL_KEY
// PLU快捷键 1--100
#define _KEY_PLU1 		1
#define _KEY_PLU2 		2
#define _KEY_PLU3 		3
#define _KEY_PLU4 		4
#define _KEY_PLU5 		5
#define _KEY_PLU6 		6
#define _KEY_PLU7 		7
#define _KEY_PLU8 		8
#define _KEY_PLU9 		9
#define _KEY_PLU10 		10
#define _KEY_PLU11 		11
#define _KEY_PLU12 		12
#define _KEY_PLU13 		13
#define _KEY_PLU14 		14
#define _KEY_PLU15 		15
#define _KEY_PLU16 		16
#define _KEY_PLU17 		17
#define _KEY_PLU18 		18
#define _KEY_PLU19 		19
#define _KEY_PLU20 		20
#define _KEY_PLU21 		21
#define _KEY_PLU22 		22
#define _KEY_PLU23 		23
#define _KEY_PLU24 		24
#define _KEY_PLU25 		25
#define _KEY_PLU26 		26
#define _KEY_PLU27 		27
#define _KEY_PLU28 		28
#define _KEY_PLU29 		29
#define _KEY_PLU30 		30
#define _KEY_PLU31 		31
#define _KEY_PLU32 		32
#define _KEY_PLU33 		33
#define _KEY_PLU34 		34
#define _KEY_PLU35 		35
#define _KEY_PLU36 		36
#define _KEY_PLU37 		37
#define _KEY_PLU38 		38
#define _KEY_PLU39 		39
#define _KEY_PLU40 		40
#define _KEY_PLU41 		41
#define _KEY_PLU42 		42
#define _KEY_PLU43 		43
#define _KEY_PLU44 		44
#define _KEY_PLU45 		45
#define _KEY_PLU46 		46
#define _KEY_PLU47 		47
#define _KEY_PLU48 		48
#define _KEY_PLU49 		49
#define _KEY_PLU50 		50
#define _KEY_PLU51 		51
#define _KEY_PLU52 		52
#define _KEY_PLU53 		53
#define _KEY_PLU54 		54
#define _KEY_PLU55 		55
#define _KEY_PLU56 		56
#define _KEY_PLU57 		57
#define _KEY_PLU58 		58
#define _KEY_PLU59 		59
#define _KEY_PLU60 		60
#define _KEY_PLU61 		61
#define _KEY_PLU62 		62
#define _KEY_PLU63 		63
#define _KEY_PLU64 		64
#define _KEY_PLU65 		65
#define _KEY_PLU66 		66
#define _KEY_PLU67 		67
#define _KEY_PLU68 		68
#define _KEY_PLU69 		69
#define _KEY_PLU70 		70
#define _KEY_PLU71 		71
#define _KEY_PLU72 		72


// 数字键 100-110
#define _KEY_NUM0   	100
#define _KEY_NUM1   	101
#define _KEY_NUM2   	102
#define _KEY_NUM3   	103
#define _KEY_NUM4   	104
#define _KEY_NUM5   	105
#define _KEY_NUM6   	106
#define _KEY_NUM7   	107
#define _KEY_NUM8   	108
#define _KEY_NUM9   	109
#define _KEY_CLR      	110
#define _KEY_DOT     	111


// 功能键 120--150
#define _KEY_F1      	120
#define _KEY_F2      	121
#define _KEY_F3     	122
#define _KEY_F4      	123
#define _KEY_F5      	124
#define _KEY_F6      	125
#define _KEY_F7      	126
#define _KEY_F8     	127
#define _KEY_F9      	128
#define _KEY_F10      	129
#define _KEY_F11      	130
#define _KEY_F12     	131
#define _KEY_F13      	132
#define _KEY_F14      	133
#define _KEY_F15      	134
#define _KEY_F16      	135
#define _KEY_F17      	136
#define _KEY_F18      	137
#define _KEY_F19      	138
#define _KEY_F20      	139
#define _KEY_F21     	140
#define _KEY_F22      	141
#define _KEY_F23      	142
#define _KEY_F24      	143
#define _KEY_F25      	144
#define _KEY_F26      	145
#define _KEY_F27      	146
#define _KEY_F28      	147
#define _KEY_F29      	148
#define _KEY_F30      	149

#define KEY_CAL                199
#define KEY_POWER 		200




#define _KEY_ZERO      			_KEY_F5
#define _KEY_TARE      			_KEY_F6	
#define _KEY_SHIFT      		_KEY_F7
#define _KEY_PLU	      		_KEY_F8
#define _KEY_MENU		      	_KEY_F9
#define _KEY_WEIGHT_MODE                _KEY_F10
#define _KEY_PAY_MODE      		_KEY_F11
#define _KEY_ADD      			_KEY_F12
#define _KEY_CANCEL		      	_KEY_F13	
#define _KEY_PAPER		      	_KEY_F14	
#define _KEY_TOTAL                      _KEY_F15
#define _KEY_PRINT      		_KEY_F16	

//重定
#define _KEY_LOGIN                      _KEY_SHIFT
#define _KEY_X                          _KEY_WEIGHT_MODE
#define _KEY_ENTER                      _KEY_PRINT
#define _KEY_UP                         _KEY_ADD
#define _KEY_DOWN                       _KEY_CANCEL
#define _KEY_BACK                       _KEY_PLU


#endif // TYPE_H

