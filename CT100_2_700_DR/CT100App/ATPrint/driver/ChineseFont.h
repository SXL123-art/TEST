#ifndef __CHS_H__
#define __CHS_H__

#ifdef USE_CHN_GB2312_24x24
#define CHN_BOLD_24x24    1
#define CHN_SONGTI_24x24  2
#define CN_WIDTH_BYTE     3
#define CN_WIDTH          24
#define DOUBLE_CN_WIDTH  (CN_WIDTH * 2)
#define CN_HEIGHT         24
#elif defined(USE_CHN_GB2312_16x16)
#define CHN_STD_16x16     1
#define CN_WIDTH_BYTE     2
#define CN_WIDTH          16
#define DOUBLE_CN_WIDTH  (CN_WIDTH * 2)
#define CN_HEIGHT         16
#endif

#ifdef USE_CHN_GB2312_24x24
extern const unsigned char ChnFont_Bold_24x24[];
#ifdef USE_CHN_SONGTI_FONT
extern const unsigned char ChnFont_Songti_24x24[];
#endif
#endif

#ifdef USE_CHN_GB2312_16x16
extern const unsigned char ChnFont_Std_16x16[];
#endif

#endif//__CHS_H__
