#define DBCS  0x40
#define S0    0x00

#include <stdio.h>
//#include "PRT_Driver.h"
//#include "common.h"
#include "./../../public/globals.h"
#include "define.h"
#include "EngFont.h"
#include "ChineseFont.h"
#include "ext_var.h"
#include "ext_fun.h"
#include "./../print.h"
#include "./../set.h"
//unsigned char state=S0;

void font_type(unsigned char type)      //type= FontNO,FontBOLD,FontWIDE,FontHIGH,FontBIG,FontUNDERLINE,FontSTRIKETHROUGH
{
    switch(type)
    {
    case FontNO:
        sprintf((char *)prnbuf, "%c%c",0x1b,0x40);
        break;
    case FontBOLD:
        sprintf((char *)prnbuf, "%c%c%c",0x1b,0x45,0x31);
        break;
    case FontWIDE:
        sprintf((char *)prnbuf, "%c%c%c",0x1b,0x55,0x31);
        break;
    case FontHIGH:
        sprintf((char *)prnbuf, "%c%c%c",0x1b,0x56,0x31);
        break;
    case FontBIG:
        sprintf((char *)prnbuf, "%c%c%c",0x1b,0x57,0x31);
        break;
    case FontUNDERLINE:
        sprintf((char *)prnbuf, "%c%c%c",0x1b,0x2d,0x31);
        break;
    case FontSTRIKETHROUGH:
        sprintf((char *)prnbuf, "%c%c%c%c",0x1b,0x28,0x2d,0x31);
        break;
    default:
        break;
    }
    print();
}

void send_first_line(void)
{
#ifndef USE_CT100G
    extern void get_online(unsigned char * str,unsigned char * dst);
    get_online(print_oneline,(uint8*)tph_prt_addr);
    tph_prt_addr += 48;

    PRT_LATCH_H();
    spi0_put_str(print_oneline);
    PRT_LATCH_L();
    tph_prt_line++;
#endif
}

void revchar_proc(unsigned char ch)					        //댔丹 뵨寧硅츱즈斤뵀털뙤
{
    //unsigned char ch_tmp;
    //unsigned char c_type;
    static char ch_tmph=0x00;

    switch(state)
    {
    case S0:
        if(ch>0xA0)                 //ASCII MAX Value is 0X7F
        {
            if ((ch>=0xa1) && (ch<= 0xf7))          //Chinese Fonts
            {
                ch_tmph=ch; //董닸櫓匡코쯤돨멕俚쌘
                state=DBCS;
            }
        }
        else if (ch>=0x20) //0x20-0x80裂쇌 亶匡
        {
            //ASCII:0x20裂품角츱즈，꼇角옵鞫刻돨俚륜
            ch_tmph=0x00;
            if (draw_x_pos + eng_font_width > 384)//흔벎狼댔丹돨俚낚법寧契,橙
            {
#ifndef USE_CT100G
                prt_memory_ctrl(FONT_HEIGHT);
                extern void darkness_speed_strobe();
                darkness_speed_strobe(darkness);
                power_printer();//侶좃훙얌 봤獗角뻣契
#else
                prt_memory_ctrl(FONT_HEIGHT);
                power_printer();//侶좃훙얌 봤獗角뻣契
#endif
            }
            make_img_eng(ch,0,0);
        }
        else
        {
            //<20 츱즈
            switch (ch)
            {
            case CR:
            case LF: //뻣契
#ifndef USE_CT100G
                //img_buf_init();
                prt_memory_ctrl(FONT_HEIGHT);
                power_printer();
#else
                prt_memory_ctrl(FONT_HEIGHT);
                power_printer();//侶좃훙얌 봤獗角뻣契
#endif
                break;
            case 0x20://왕목
                draw_x_pos+=eng_font_width;//draw_x_pos藤속寧몸俚륜욱
                break;
            case HT: //TAB숩:4몸俚쌘
                draw_x_pos+=32;     // 8*4
                break;
            case ESC:
                state = ESC_CMD;//硫구쌈澗ESC돨宮밑츱즈
                //start_command_delay();//零ESC츱즈儺珂珂쇌
                break;
            case FS:
                state = FS_CMD;
                //start_command_delay(); // Start esc command delay
                break;
            default:
                break;
            }
        }
        break;
#ifdef COUNTRY_CHINA
    case DBCS://櫓匡榴檄 //댔丹櫓匡
        if (draw_x_pos+cn_font_width>PRT_MAX_WIDTH)  //낚법寧契,橙버땡寧契
        {
#ifndef USE_CT100G
            prt_memory_ctrl(CN_HEIGHT);  //놓迦뺏
            extern void darkness_speed_strobe();
            darkness_speed_strobe(darkness);
            power_printer();    //疼버땡寧匡俚契
#else
            prt_memory_ctrl(CN_HEIGHT);  //놓迦뺏
            power_printer();//侶좃훙얌 봤獗角뻣契
#endif
        }
        if ((ch>=0xa1) && (ch<=0xfe))
            make_img_eng(ch_tmph,ch,1);  //ch_tmph槨뒤寧늴澗돨櫓匡코쯤멕俚쌘,ch槨먼혤놔돨코쯤됴俚쌘
        state=S0;
        ch_tmph=0x00;
        break;
#endif
    case ESC_CMD:
        state=S0;
        //shut_command_delay();// Shut esc command delay
        if (ch=='W') state=ESC_W;//憐竟렴댕     0x57
        if (ch=='V') state=ESC_V;//李蕨렴댕		0x56
        if (ch=='U') state=ESC_U;//뷘蕨렴댕		0x55
        if (ch=='-') state=ESC_m;//苟뺍窟 		0x2D  // PRT_TYPE_UNDERLINE Command State
        if (ch=='E') state=ESC_E;//닛竟 		0x45  // PRT_TYPE_UNDERLINE Command State
        if (ch=='(') state=ESC_p;//뇜窟			0x28
        if (ch=='1') state=ESC_1;//땍契쇌약
        if (ch=='K') state=ESC_K;//댔丹菱땍屢듐黎暠
        //start_command_delay();// Start esc command delay
        if (ch=='@') 						//  0X40
        {
            //놓迦뺏
            printer_init();
            //shut_command_delay();
        }
        if (ch=='T') state=ESC_T;//Traceability Code
        break;
    case ESC_W://렴댕俚竟
        state=S0;
        //shut_command_delay(); // Shut esc command delay
        if (ch == '0')
        {
            eng_font_width=FONT_WIDTH;
#ifdef COUNTRY_CHINA
            cn_font_width =CN_WIDTH;
#endif
            prt_ctrl_width=0;
            prt_ctrl_height=0;
        }
        else if (ch == '1')
        {
            eng_font_width = DOUBLE_FONT_WIDTH;
#ifdef COUNTRY_CHINA
            cn_font_width = DOUBLE_CN_WIDTH;
#endif
            prt_ctrl_width=1;
            prt_ctrl_height=1;
        }
        break;
    case ESC_V://李蕨렴댕
        state=S0;
        //shut_command_delay();// Shut esc command delay
        if ((ch == '0') || (ch == '1'))
        {
            prt_ctrl_width=0;
            prt_ctrl_height=ch-'0';//=0샀1

            eng_font_width = FONT_WIDTH;//긴槨鬼俚竟
#ifdef COUNTRY_CHINA
            cn_font_width = CN_WIDTH;
#endif
        }
        break;
    case ESC_U:
        state=S0;
        // shut_command_delay();// Shut esc command delay
        if (ch == '0')
        {
            eng_font_width=FONT_WIDTH;
#ifdef COUNTRY_CHINA
            cn_font_width=CN_WIDTH;
#endif
            prt_ctrl_width=0;
            prt_ctrl_height=0;
        }
        else if (ch == '1')
        {
            eng_font_width = DOUBLE_FONT_WIDTH;
#ifdef COUNTRY_CHINA
            cn_font_width = DOUBLE_CN_WIDTH;
#endif
            prt_ctrl_width=1;//뷘蕨렴댕죄
            prt_ctrl_height=0;
        }
        break;
    case ESC_m: // PRT_TYPE_UNDERLINE Command   //苟뺍窟
        state=S0;
        // shut_command_delay();// Shut esc command delay
        if (ch == '1') prt_ctrl_typeface |= PRT_TYPE_UNDERLINE;
        else if (ch == '0') prt_ctrl_typeface &= (~PRT_TYPE_UNDERLINE);
        break;
    case ESC_E: // PRT_TYPE_BOLD Command       //닛竟
        state=S0;
        //shut_command_delay(); // Shut esc command delay
        if (ch == '1') prt_ctrl_typeface |= PRT_TYPE_BOLD;
        else if (ch == '0') prt_ctrl_typeface &= (~PRT_TYPE_BOLD);
        break;
    case ESC_p: //뇜窟
        state=S0;
        //shut_command_delay();// Shut esc command delay
        if (ch == '-') state=ESC_p_m;
        //start_command_delay(); // Start esc command delay
        break;
    case ESC_p_m:
        state=S0;
        // shut_command_delay();// Shut esc command delay
        if (ch == '0')
        {
            //혼뇜窟
            prt_ctrl_typeface &= (~PRT_TYPE_STROKE);
            prt_ctrl_typeface &= (~PRT_TYPE_DOUBLE_STROKE);
        }
        else if (ch == '1')
        {
            //데뇜窟
            prt_ctrl_typeface |= PRT_TYPE_STROKE;
            prt_ctrl_typeface &= (~PRT_TYPE_DOUBLE_STROKE);
        }
        else if (ch == '2')
        {
            //崗뇜窟
            prt_ctrl_typeface |= PRT_TYPE_DOUBLE_STROKE;
            prt_ctrl_typeface &= (~PRT_TYPE_STROKE);
        }
        break;
    case ESC_1://땍契쇌약
        state=S0;
        // shut_command_delay();// Shut esc command delay
        space_line=ch-'0';
        break;
    case ESC_T:
        if (draw_x_pos + eng_font_width > 384)//흔벎狼댔丹돨俚낚법寧契,橙
        {
#ifndef USE_CT100G
            prt_memory_ctrl(FONT_HEIGHT);
            extern void darkness_speed_strobe();
            darkness_speed_strobe(darkness);
            power_printer();//侶좃훙얌 봤獗角뻣契
#else
            prt_memory_ctrl(FONT_HEIGHT);
            power_printer();//侶좃훙얌 봤獗角뻣契
#endif
        }
        make_tracecode(ch);
        //state=S0;
        break;
    default:
        break;
    }
}
