#ifndef __ENGFONT_H__
#define __ENGFONT_H__
#include "./../../public/globals.h"

#ifdef USE_ENG_12x24
#define ENG_BOLD_12x24     1
#define ENG_SONGTI_12x24   2
#define ENG_ARIAL_12x24    3
#define FONT_WIDTH_BYTE    2
#define FONT_WIDTH         12
#define FONT_HEIGHT        24
#define DOUBLE_FONT_WIDTH  FONT_WIDTH * 2
#elif defined(USE_ENG_8x16)
#define ENG_STD_8x16       1
#define FONT_WIDTH_BYTE    1
#define FONT_WIDTH         8
#define FONT_HEIGHT        16
#define DOUBLE_FONT_WIDTH  FONT_WIDTH * 2
#endif

#ifdef USE_ENG_12x24
extern const unsigned char EngFont_Arial_12x24[];
extern const unsigned char EngFont_Bold_12x24[];
extern const unsigned char EngFont_Songti_12x24[];

#elif defined(USE_ENG_8x16)
extern const unsigned char EngFont_Std_8x16[];
#endif

#ifdef COUNTRY_CHINA
extern const unsigned char TraceFont[];
#endif

#endif
