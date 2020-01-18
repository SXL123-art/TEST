#define IMG_DEF
//#define CHS_DEF

//#include "./../../init.h"
#include "./../../public/globals.h"
#include "./../print.h"
#include "ChineseFont.h"
#include "EngFont.h"
#include "ext_var.h"
#include "define.h"
#include "./../set.h"

#include "PRT_Driver.h"
//#include "init.h"

//#include "comm_app.h"


/*
void CommInterpreter(HUGEDATA COMM_BUF *CBuf)
{
    PACKET_HEADER *header;
    PACKET_TAIL *tail;
    INT16U length;
    INT8U  checksum;
    INT16U i;
    INT8U errNo;
    INT16U rxSize;
    INT16U sendLength;
    INT16U check_end_pos;
    INT8U opCode;

    sendLength = 0;

    rxSize = GetRxBufSize(CBuf);
    GetDataRxBuf(CBuf, MsgBuf, rxSize);

    if ((rxSize) < sizeof(PACKET_HEADER))
    {
        return;
    }

    header = (PACKET_HEADER *)MsgBuf;
    opCode = header->opCode[1];
    length = header->length;
    printf("length is and opCode is%d,%c,\n",length,opCode);
    if (header->opCode[1] == DATA_PLU)
    {
        if (length > (PLU_HOST_STORAGE_SIZE + DMSG_HOST_STORAGE_SIZE))
        {
            errNo = ERROR_DATA;
            goto SEND_ERR;
        }
    }

    if (header->colon != ':')							// ÆÐÅ¶ header È®ÀÎ
    {
        goto ERR_HANDLER;
    }
    else if (header->delilimeter != ',')
    {
        goto ERR_HANDLER;
    }
#ifdef USE_USB_HOST
    else if (('W' != header->opCode[0]) && ('R' != header->opCode[0]) &&
             ('G' != header->opCode[0]) && ('N' != header->opCode[0]))
#else
    else if (('W' != header->opCode[0]) && ('R' != header->opCode[0]))
#endif
    {
        goto ERR_HANDLER;
    }

    if (length > COM_MAX_RX_BUF_HOST_SIZE)
    {
        //goto ERR_HANDLER;
        goto ERR_HANDLER_CHECKSUM;                 //poland err
    }

    if (rxSize < length + sizeof(PACKET_HEADER) + sizeof(PACKET_TAIL))
    {
        return;
    }

    tail = (PACKET_TAIL *)&MsgBuf[length + sizeof(PACKET_HEADER)];

    if (tail->colon != ':')							// ÆÐÅ¶ tail È®ÀÎ
    {
        //goto ERR_HANDLER;
        printf("tail->colon is not ':',but %c\n",tail->colon);
        goto ERR_HANDLER_CHECKSUM;
    }
    else if (tail->cr != ASC_CR)
    {
        //goto ERR_HANDLER;
        printf("tail->cr is not 'oxod',but %d\n",tail->cr);
        goto ERR_HANDLER_CHECKSUM;
    }

    checksum = 0;

    i = 2;
    check_end_pos = length + sizeof(PACKET_HEADER);//data ¸¶Áö¸· ±îÁö
    printf("end pos is %d \n",check_end_pos);
    while(i < check_end_pos)
    {
        checksum += MsgBuf[i++];
    }

    checksum += ':';//tail ':' ±îÁö checksum °è»ê

    if (tail->checksum != checksum)
    {
        printf("scale checksum is %d,but pc checksum is %d\n",checksum,tail->checksum);
        //goto ERR_HANDLER;
        goto ERR_HANDLER_CHECKSUM;
    }

    //TO DO : MsgBuf¸¦ Input, Output ¸ðµÎ »ç¿ëÇÏ°í ÀÖÀ½, ¹«°á¼º °Ë»ç ÇÊ¿äÇÔ
    if (header->opCode[0] == 'W')
    {
        errNo = writeReceivedData((INT8U *)MsgBuf, &sendLength);
    }
    else if (header->opCode[0] == 'R')
    {
        errNo = readSavedData((INT8U *)MsgBuf, &sendLength);
    }
#ifdef USE_USB_HOST
    else if (header->opCode[0] == 'G' || header->opCode[0] == 'N')
    {
        errNo = checkUsbHostData((INT8U *)MsgBuf, &sendLength);
        if (errNo != 0) goto SEND_ERR;
        return;
    }
#endif
    else
    {
        errNo = ERROR_INTERPRETER;
    }

    if (errNo == 0 || errNo == LONG_EXE_ERASE_ALL_PLU)
    {
        WindRxBuf(CBuf, length + sizeof(PACKET_HEADER) + sizeof(PACKET_TAIL));
        PutDataTxBuf(CBuf, MsgBuf, sendLength);
        if (errNo == LONG_EXE_ERASE_ALL_PLU)
        {
            network_tx_proc();
            //plu_init(); need replacing
        }
        return;
    }

SEND_ERR:
    MsgBuf[0]='N';
    MsgBuf[1]=opCode;
    if (opCode != DATA_FLASH && opCode != DATA_EEPROM && opCode != DATA_RAM)
    {
        MsgBuf[2]=(settings.scaleNo)&0x00ff; //0;//cstruct.scaleAddr; need replacing
        MsgBuf[3]=((settings.scaleNo)&0xff00) >> 8;//0;
        MsgBuf[4]=0;
        MsgBuf[5]=0;
    }
    MsgBuf[6]=',';
    MsgBuf[7]=5;
    MsgBuf[8]=0;
    MsgBuf[9]=':';
//  ±âÁ¸¿¡ ¹ÞÀº ¹øÈ£ ±×´ë·Î ¹ÝÈ¯ÇÏ°Å³ª
//	ÇØ¼®±â¿¡¼­ modify Çß´Ù°í °¡Á¤ÇÔ
//	MsgBuf[10]=0;
//	MsgBuf[11]=0;
//	MsgBuf[12]=0;
//	MsgBuf[13]=0;
    MsgBuf[14]=errNo;
    MsgBuf[15]=':';

    checksum = 0;

    for(i=2; i<16; i++) checksum += MsgBuf[i];

    MsgBuf[16]=checksum;
    MsgBuf[17]=ASC_CR;

    FlushRxBuf(CBuf);
    PutDataTxBuf(CBuf, MsgBuf, 18);
    return;

ERR_HANDLER:
    if (TRUE == findNextPacketHeader(CBuf, (INT8U *)MsgBuf, rxSize))
    {
        return;
    }
    else
    {
ERR_HANDLER_CHECKSUM:
        errNo = ERROR_RECEIVE;
        goto SEND_ERR;
    }
}

*/
//extern const unsigned char EngFont[2054];


/*******************************************************************************
* Function Name  : ASCNUM2BOLDFACE
* Description    : ASCII numeric change to Chinese BoldFace,For the CHS Market's traceability code need BoldFace.
* Input          : ASCII numeric string
* Output         : None
* Return         : CHS numeric string
example:
    sprintf((char *)prnbuf, ASCNUM2BOLDFACE("1234567890"));
    print();
    tireta();
*******************************************************************************/
unsigned char * ASCNUM2BOLDFACE(unsigned char * p)
{
    unsigned char i,len;
    len = strlen((char const *)p);

    for(i=0; i<24; i++)
    {
        TraceabilityCode[i]=0;
    }

    for(i=0; i<len; i++)
    {
        TraceabilityCode[i*2+1] = *(p+i) + 128;
        TraceabilityCode[i*2] = 0xA3;
    }
    return TraceabilityCode;

}

char width_h(char ch)                  //高字节字体变宽
{
    SFR8 tmp;
    tmp.Bit.b7=tmp.Bit.b6=(ch&0x80)>>7;
    tmp.Bit.b5=tmp.Bit.b4=(ch&0x40)>>6;
    tmp.Bit.b3=tmp.Bit.b2=(ch&0x20)>>5;
    tmp.Bit.b1=tmp.Bit.b0=(ch&0x10)>>4;
    return tmp.DATA;
}

char width_l(char ch)                  //低字节字体变宽
{
    SFR8 tmp;
    tmp.Bit.b7=tmp.Bit.b6=(ch&0x08)>>3;
    tmp.Bit.b5=tmp.Bit.b4=(ch&0x04)>>2;
    tmp.Bit.b3=tmp.Bit.b2=(ch&0x02)>>1;
    tmp.Bit.b1=tmp.Bit.b0=(ch&0x01);
    return tmp.DATA;
}

void img_buf_init(void)                //img_buf的高度，地址，指针等初始化
{
    unsigned short i;

    for (i=0; i<0x900; i++)
    {
        // Initialize img_buf as 0x00
        BUF0[i]=BUF1[i]= 0x00;                    //外部SRAM的0x0000-0x0600或0x0600-0x0C00全写0		//从0X0000到0X0600=48*32BYTE

    }
    img_buf0.height = FONT_HEIGHT;				//高度=16
    img_buf0.prt_addr = img_buf0.write_addr = &BUF0[0x300];				//地址为0X0300或0X0900
    img_buf0.prt_line =0;

    img_buf1.height = FONT_HEIGHT;				//高度=16
    img_buf1.prt_addr = img_buf1.write_addr = &BUF1[0x300];				//地址为0X0300或0X0900
    img_buf1.prt_line =0;
}


//IMG_BUF* prt_thp_buffer;

//unsigned char space_line=3;								//行距
//unsigned char prt_state=0;								//打印状态:应该是正在打印、打印空闲、等  //=0X80时正在打印，=0时没有打印


void printer_init(void)			//初始化img_buf0,img_buf1，和打印字体型
{
#ifndef USE_CT100G
    while(prt_state&0x80);                                     // Wait for printing finished
#endif
    img_buf_init();
    draw_x_pos = 0;
    eng_font_width = FONT_WIDTH;                                      // English Font Normal Size
#ifdef COUNTRY_CHINA
    cn_font_width  = CN_WIDTH;                                      // Chinese Font Normal Size
#endif
    prt_img_buffer=&img_buf0;

    prt_ctrl_typeface=0;                                        // NORMAL FONT
    //prt_ctrl_width=1;                                           // width =0 is default size
    prt_ctrl_width=0;
    //prt_ctrl_height=1;                                          // height=0 is default size
    prt_ctrl_height=0;

#ifdef USE_ENG_12x24
    space_line=3;
#else
    space_line=1;                                               // space_line=3 is default height
#endif

    prt_state=0;
    prt_read=0;
    prt_write=0;
    state=S0;
}

void prt_memory_ctrl(unsigned char heigh)								//一些打印寄存器的设置
{
#ifndef USE_CT100G
    while(prt_state&0x80);							//如果正在打印则等
#endif
    if (prt_state==0)
    {
        //如果没在打印,则 IMG_buf0
        tph_prt_line = img_buf0.prt_line;
        tph_prt_addr = (unsigned int)img_buf0.prt_addr;
        tph_prt_height = img_buf0.height;
        prt_img_buffer = &img_buf1;
    }
    else
    {
        tph_prt_line = img_buf1.prt_line;
        tph_prt_addr = (unsigned int)img_buf1.prt_addr;
        tph_prt_height = img_buf1.height;
        prt_img_buffer = &img_buf0;
    }

    prt_img_buffer->prt_line=0;
    prt_img_buffer->prt_addr = prt_img_buffer->write_addr;                      // Set prt_img Buffer's printed address as write_addr
    prt_img_buffer->height = heigh;

    draw_x_pos=0;
    prt_state|=0x80;
}

#ifdef COUNTRY_CHINA//USE_CHN_GB2312_16x16
unsigned char eng_font_img[CN_WIDTH_BYTE * FONT_HEIGHT];
#else
unsigned char eng_font_img[FONT_WIDTH_BYTE * FONT_HEIGHT];
#endif
void typeface_font(unsigned char language,unsigned char typeface)
{
    if(language==0)
    {
#ifdef USE_ENG_12x24
        if (typeface&PRT_TYPE_UNDERLINE)    			//如果有下划线    //0x7F,0xE0而不是0xFF是要前后空两点  不然全打黑电流大
        {
            eng_font_img[46]|= 0x7f;							//有下划线，则最下一行全或上1； 打印出来就全是点，即下划线
            eng_font_img[47]|= 0xE0;
            eng_font_img[44]|= 0x7f;							//有下划线，则最下一行全或上1； 打印出来就全是点，即下划线
            eng_font_img[45]|= 0xE0;
        }
        if (typeface&PRT_TYPE_STROKE)
        {
            eng_font_img[22]|= 0x7f;     					//有删除线，则中间一行全或上1；
            eng_font_img[23]|= 0xE0;
            eng_font_img[20]|= 0x7f;     					//有删除线，则中间一行全或上1；
            eng_font_img[21]|= 0xE0;
        }
        if (typeface&PRT_TYPE_DOUBLE_STROKE)       //如果有双删除线，则[7]，[9]行全或上1；
        {
            eng_font_img[18]|= 0x7f;
            eng_font_img[19]|= 0xE0;
            eng_font_img[26]|= 0xff;
            eng_font_img[27]|= 0xE0;
        }
#else
        if (typeface&PRT_TYPE_UNDERLINE)
        {
            eng_font_img[30]|= 0x7f;
            eng_font_img[31]|= 0xf7;
        }
        if (typeface&PRT_TYPE_STROKE)
        {
            eng_font_img[14]|= 0x7f;
            eng_font_img[15]|= 0xf7;
        }
        if (typeface&PRT_TYPE_DOUBLE_STROKE)
        {
            eng_font_img[12]|= 0x7f;
            eng_font_img[13]|= 0xf7;
            eng_font_img[16]|= 0x7f;
            eng_font_img[17]|= 0xf7;
        }
#endif
    }
    else
    {
        if (typeface&PRT_TYPE_UNDERLINE)
        {
            eng_font_img[30]|= 0x7f;
            eng_font_img[31]|= 0xf7;
        }
        if (typeface&PRT_TYPE_STROKE)
        {
            eng_font_img[14]|= 0x7f;
            eng_font_img[15]|= 0xf7;
        }
        if (typeface&PRT_TYPE_DOUBLE_STROKE)
        {
            eng_font_img[12]|= 0x7f;
            eng_font_img[13]|= 0xf7;
            eng_font_img[16]|= 0x7f;
            eng_font_img[17]|= 0xf7;
        }
    }
}


void make_img_eng(unsigned char ch,unsigned char cl,unsigned char language)                                             //根据参数ch,从flash里读出这个英文的点阵表到eng_font_img[16]里,再根据字体高宽,粗,下划线,删除线等格式转变这个eng_font_img[16]里的点阵表,最后存到外部SRAM的0x0000-0x0600区域里
{
    unsigned char j;
    unsigned long flash_dst=0;
    unsigned char* addr;
    unsigned short buf_pos;
#ifdef USE_ENG_12x24
    unsigned short temp = 0; //KYK test
#endif
    unsigned char eng_byte0;
    unsigned char eng_byte1;
    unsigned char tmp_byte0;
    unsigned char tmp_byte1;
#ifdef USE_CHN_GB2312_24x24
    unsigned char eng_byte2,tmp_byte2;
#endif

    unsigned char typeface_tmp;
    unsigned char height_tmp;
    unsigned char width_tmp;
    unsigned char ch_tmph,ch_tmpl;

    typeface_tmp=prt_ctrl_typeface;
    height_tmp=prt_ctrl_height;
    width_tmp=prt_ctrl_width;
    //SPACE的ASCII码为0X20

    if(language==1)         //CHS
    {
#ifdef USE_CHN_GB2312_24x24
        ch_tmph=ch-0xA1;
        ch_tmpl=cl-0xA1;
        flash_dst=(ch_tmph*94 + ch_tmpl)*3*24;                                    //calculate the Chinese font's address in the Font Form;
        for(j=0; j<72; j++)
        {
            switch(settings.chnFontNo)//牢尖加档 绊妨秦具 窃
            {
            default:
            case CHN_BOLD_24x24:
                eng_font_img[j]=ChnFont_Bold_24x24[flash_dst++];
                break;
            case CHN_SONGTI_24x24:
#ifdef USE_CHN_SONGTI_FONT
                eng_font_img[j]=ChnFont_Songti_24x24[flash_dst++];
#else
                eng_font_img[j]=ChnFont_Bold_24x24[flash_dst++];
#endif
                break;
            }
            //eng_font_img[j]=CHS16[flash_dst++];
        }
#elif defined(USE_CHN_GB2312_16x16)
        ch_tmph=ch-0xA1;
        ch_tmpl=cl-0xA0;
        flash_dst=(ch_tmph*96 + ch_tmpl)*32;                                    //calculate the Chinese font's address in the Font Form;
        for(j=0; j<32; j++)
        {
            //eng_font_img[j]=CHS16[flash_dst++];
            eng_font_img[j]=ChnFont_Std_16x16[flash_dst++];
        }
#endif
    }
    else    //English Fonts
    {
        flash_dst=(ch-0x20)*48;

#ifdef USE_ENG_8x16
        flash_dst <<=4;
#endif
        for (j=0;  j<FONT_WIDTH_BYTE*FONT_HEIGHT; j++)
        {
            //读16次
#ifdef USE_ENG_12x24
           switch(settings.engFontNo)
            {
            default:
#ifndef USE_CT100G
            case ENG_BOLD_12x24:
                eng_font_img[j]= EngFont_Bold_12x24[flash_dst++];
                break;
            case ENG_SONGTI_12x24:
                eng_font_img[j]= EngFont_Songti_12x24[flash_dst++];
                break;
            case ENG_ARIAL_12x24:
                eng_font_img[j]= EngFont_Arial_12x24[flash_dst++];
                break;
#else
                //case ENG_ARIAL_12x24: //test
                eng_font_img[j]= EngFont_Arial_12x24[flash_dst++];
                break;
#endif
            }
#else
            switch(settings.engFontNo)
            {
            default:
            case ENG_STD_8x16:
                eng_font_img[j]= EngFont_Std_8x16[flash_dst++];
                break;
            }
#endif
        }
    }
    addr = prt_img_buffer->write_addr ;//*.write_addr=0x0300或0X0900
    if (height_tmp == 1) //本来一个英文占16个字符,如果高度为1,则占32个字符,这样48个英文占32*48=1536 =0X0600
    {
        //addr -= 0x0300;//KYK test                 //减后addr=0x0或0x0600
        prt_img_buffer->prt_addr=addr;  //img地址归位 =0X0或0x600
#ifdef USE_CHN_GB2312_24x24
        prt_img_buffer->height=FONT_WIDTH_BYTE * FONT_HEIGHT;//32;//40;              //实际变高后高度为36，设为40就是行距大些，打印美观些。
#else
        prt_img_buffer->height=32;
#endif

    }
    typeface_font(language,typeface_tmp);
    if(language==0)
    {
#ifdef USE_ENG_12x24
        buf_pos=draw_x_pos/12;       //>>3;				//写到RAM里时的位置记录	//初始时draw_x_pos=0; 相当于除以8，大字体时:第二次时(执行第291行后),buf_pos=2,第三次时=4；小字体时:第二次时buf_pos=1,第三次时=3;    //2011.2.18改为除12，因字库换了
        temp=buf_pos+buf_pos/2;								//addr[temp]写的指针计算，即buf_pos的3/2
        if(draw_x_pos % 12 )
        {
            temp++;
        }
#else
        buf_pos=draw_x_pos>>3;
#endif
    }
    else            //CHS
    {
        buf_pos=draw_x_pos>>3;      // x/8
#ifdef USE_ENG_12x24
        if(draw_x_pos %8 ) buf_pos++;
#endif
    }
#ifdef USE_CHN_GB2312_16x16
    for (j=0; j<48 ; j++)
#elif defined (USE_CHN_GB2312_24x24)
    for (j=0; j<72 ; j++)//将一个英文字符写到RAM里   //FONT_WIDTH_BYTE*FONT_HEIGHT
#else
    for (j=0; j<FONT_WIDTH_BYTE*FONT_HEIGHT ; j++)//将一个英文字符写到RAM里   //FONT_WIDTH_BYTE*FONT_HEIGHT
#endif
    {
        if(language==1)         //One Chinese word only have 32 bytes
        {
#ifdef USE_CHN_GB2312_16x16
            if(j==32) break;
#endif
        }
        if(language==0)         //One Chinese word only have 32 bytes
        {
            if(j==FONT_WIDTH_BYTE * FONT_HEIGHT) break;
        }
        if(language==0)         //ENG
        {
#ifdef USE_ENG_12x24
            eng_byte0=eng_font_img[j++];		//low byte
            eng_byte1=eng_font_img[j];   	//heigh byte
#elif defined(USE_ENG_8x16)
            eng_byte0=eng_font_img[j];
#endif
        }
        else            //CHS
        {
            eng_byte0=eng_font_img[j++];		//low byte
            eng_byte1=eng_font_img[j];   	//heigh byte
#ifdef USE_CHN_GB2312_24x24
            eng_byte2=eng_font_img[++j];
#endif
        }
        if (typeface_tmp&PRT_TYPE_BOLD)    //粗体   		//粗体方法: 字符的每个字节向右边扩一个点
        {
            if(language==0)     //ENG
            {
#ifdef USE_ENG_12x24
                tmp_byte1=eng_byte1>>1;
                if(eng_byte0 & 0x01) tmp_byte1 |=0x80;
                tmp_byte0=eng_byte0>>1;
                eng_byte0 |=tmp_byte0;
                eng_byte1 |=tmp_byte1;
#else
                tmp_byte0 = eng_byte0 << 1;
                eng_byte0 |= tmp_byte0;
#endif
            }
            else              //CHS
            {
#ifdef USE_CHN_GB2312_24x24
                tmp_byte2=eng_byte2>>1;
                if(eng_byte1 & 0x01) tmp_byte2 |=0x80;
                tmp_byte1=eng_byte1>>1;
                if(eng_byte0 & 0x01) tmp_byte1 |=0x80;
                eng_byte0 |=tmp_byte0;
                eng_byte1 |=tmp_byte1;
                eng_byte2 |=tmp_byte2;
#elif defined(USE_CHN_GB2312_16x16)
                tmp_byte1=eng_byte1>>1;
                if(eng_byte0 & 0x01) tmp_byte1 |=0x80;
                tmp_byte0=eng_byte0>>1;
                eng_byte0 |=tmp_byte0;
                eng_byte1 |=tmp_byte1;
#endif
            }
        }

        if (width_tmp==0)
        {
            if(language==0)     //ENG
            {
#ifdef USE_ENG_12x24
                //-----------------				            //因字体最终放到RAM里应为12*24格式，而字库里的为16*24，故要进行如下处理
                if (width_tmp==0)
                {
                    if(buf_pos%2==0)                       //if Even
                    {
                        addr[temp]= eng_byte0;			      //如果宽度为默认,则写回RAM里
                        addr[temp+1]=eng_byte1;
                        temp+=48;
                    }
                    else
                    {
                        addr[temp] &=0xf0;
                        addr[temp] |=(eng_byte0>>4)&0x0f;
                        addr[temp+1]=eng_byte0<<4;
                        addr[temp+1] |=eng_byte1>>4;
                        temp+=48;
                    }
                }
                //-----------------
#else
                addr[buf_pos]  = eng_byte0;
                buf_pos+=48;
#endif
            }
            else        //CHS
            {
                if(draw_x_pos%8==0)                       //if Even
                {
                    addr[buf_pos]= eng_byte0;
                    addr[buf_pos+1]=eng_byte1;
#ifdef USE_CHN_GB2312_24x24
                    addr[buf_pos+2]= eng_byte2;
#endif
                    buf_pos+=48; //48篮 TPH MAX dots 384 / 8俊 措茄 巴烙.
                }
                else
                {
                    addr[buf_pos-1] &=0xf0;
                    addr[buf_pos-1] |=(eng_byte0>>4)&0x0f;
                    addr[buf_pos] = eng_byte0<<4;
                    addr[buf_pos] |= eng_byte1>>4;
                    addr[buf_pos] &=0xf0;
                    addr[buf_pos] |=(eng_byte1>>4)&0x0f;
                    addr[buf_pos+1] = eng_byte1<<4;
#ifdef USE_CHN_GB2312_24x24
                    addr[buf_pos+1] |= eng_byte2>>4;
                    addr[buf_pos+2] |= eng_byte2<<4;
#endif
                    buf_pos+=48;//48篮 TPH MAX dots 384 / 8俊 措茄 巴烙.
                }
            }             //因为THP一行为48个字节,一个字符第二行,就要放到THP对应的第二行,故是从48位置开始(看草稿)
        }

        if (width_tmp==1)
        {
            //如果宽度变大,放宽方法看笔记；
            if(language==0)     //ENG
            {
#ifdef USE_ENG_12x24
                addr[temp]  = width_h(eng_byte0);
                addr[temp+1]= width_l(eng_byte0);	//放宽后一个英文字符要用24*24bits表示；
                addr[temp+2]= width_h(eng_byte1);
                temp+=48;									//字符的第二行放到TPH的第二行；
#elif defined(USE_ENG_8x16)
                addr[buf_pos]  =width_h(eng_byte0);
                addr[buf_pos+1]=width_l(eng_byte0);
                buf_pos+=48;
#endif
            }
            else                //CHS
            {
                addr[buf_pos]  = width_h(eng_byte0);
                addr[buf_pos+1]= width_l(eng_byte0);
                addr[buf_pos+2]= width_h(eng_byte1);
                addr[buf_pos+3]= width_l(eng_byte1);
#ifdef USE_CHN_GB2312_24x24
                addr[buf_pos+4]= width_h(eng_byte2);
                addr[buf_pos+5]= width_l(eng_byte2);
#endif
                buf_pos+=48;									//字符的第二行放到TPH的第二行；
            }
        }

        if (height_tmp==1)//如果高度为1,则在下一行再写一次相同的 ////因为THP一行为48个字节,一个字符第二行,就要放到THP对应的第二行,如果高度为1,则复制第一行的点阵到第二行,即第一第二行重复,第三第四行重复,依次类推,这就是文字变高的方法;
        {
            if(language==0)     //ENG
            {
#ifdef USE_ENG_12x24
#ifdef USE_CHN_GB2312_24x24
                //if(j%4==1) //KYK test                          //这里字体变高为放大1.5倍，所以只将奇数（或偶数）行复制一次，达到放高1.5倍的效果，又因上面有个J++，所以这里用了J%4==0.（如果去掉这一句则放高两倍）
#endif
                {
                    if (width_tmp==0)                   //如果高度=1，宽度=0  //因为上面temp已加过了48，这里就是下一行的了。
                    {
                        if(buf_pos%2==0)                //这段重复了上面变宽的程序
                        {
                            addr[temp]= eng_byte0;			//如果宽度为默认,则写回RAM里
                            addr[temp+1]=eng_byte1;
                            temp+=48;
                        }
                        else
                        {
                            addr[temp] &=0xf0;
                            addr[temp] |=(eng_byte0>>4)&0x0f;
                            addr[temp+1]=eng_byte0<<4;
                            addr[temp+1] |=eng_byte1>>4;
                            temp+=48;
                        }
                    }
                    if (width_tmp==1)
                    {
                        //如果高度，宽度都=1，则再复制一行
                        addr[temp]  = width_h(eng_byte0);
                        addr[temp+1]= width_l(eng_byte0);
                        addr[temp+2]= width_h(eng_byte1);
                        temp+=48;
                    }
                }
#elif defined(USE_ENG_8x16)
                if (width_tmp==0)
                {
                    addr[buf_pos]  = eng_byte0;
                    buf_pos+=48;
                }
                else if(width_tmp==1)
                {
                    addr[buf_pos]  =width_h(eng_byte0);
                    addr[buf_pos+1]=width_l(eng_byte0);
                    buf_pos+=48;
                }
#endif
            }
            else             //CHS
            {
#ifdef USE_CHN_GB2312_24x24
                // if((j%4==3)||(j%8==1))
                {
                    if (width_tmp==0)
                    {
                        if(draw_x_pos%8==0)                       //if Even
                        {
                            addr[buf_pos]= eng_byte0;			      //如果宽度为默认,则写回RAM里
                            addr[buf_pos+1]=eng_byte1;
                            addr[buf_pos+2]= eng_byte2;
                            buf_pos+=48; //48篮 TPH MAX dots 384 / 8俊 措茄 巴烙.
                        }
                        else
                        {
                            addr[buf_pos-1] &=0xf0;
                            addr[buf_pos-1] |=(eng_byte0>>4)&0x0f;
                            addr[buf_pos] = eng_byte0<<4;
                            addr[buf_pos] |= eng_byte1>>4;
                            addr[buf_pos] &=0xf0;
                            addr[buf_pos] |=(eng_byte1>>4)&0x0f;
                            addr[buf_pos+1] = eng_byte1<<4;
                            addr[buf_pos+1] |= eng_byte2>>4;
                            addr[buf_pos+2] |= eng_byte2<<4;
                            buf_pos+=48;//48篮 TPH MAX dots 384 / 8俊 措茄 巴烙.
                        }
                    }
                    else if(width_tmp==1)
                    {
                        addr[buf_pos]  = width_h(eng_byte0);
                        addr[buf_pos+1]= width_l(eng_byte0);
                        addr[buf_pos+2]= width_h(eng_byte1);
                        addr[buf_pos+3]= width_l(eng_byte1);
                        addr[buf_pos+4]= width_h(eng_byte2);
                        addr[buf_pos+5]= width_l(eng_byte2);
                        buf_pos+=48;
                    }
                    else ;
                }

#else
                // if((j%4==3)||(j%8==1))                          //这里字体变高为放大1.5倍，所以只将奇数（或偶数）行复制一次，达到放高1.5倍的效果，又因上面有个J++，所以这里用了J%4==0.（如果去掉这一句则放高两倍）
                {
                    if (width_tmp==0)
                    {
                        addr[buf_pos]  = eng_byte0;
                        addr[buf_pos+1]= eng_byte1;
                        buf_pos+=48;
                    }
                    else if(width_tmp==1)
                    {
                        addr[buf_pos]  = width_h(eng_byte0);
                        addr[buf_pos+1]= width_l(eng_byte0);
                        addr[buf_pos+2]= width_h(eng_byte1);
                        addr[buf_pos+3]= width_l(eng_byte1);
                        buf_pos+=48;
                    }
                    else ;
                }
#endif
            }
        }
    }
    if(language==0)
    {
#if defined(USE_ENG_8x16) || defined(USE_ENG_12x24)
        draw_x_pos += eng_font_width;
#endif
    }
    else
    {
#ifdef COUNTRY_CHINA
        draw_x_pos += cn_font_width;
#endif
    }
}

void make_tracecode(unsigned char num)   //traceability code 16*16
{
    unsigned char j;
    unsigned long flash_dst=0;
    unsigned char* addr;
    unsigned short buf_pos;

    unsigned char eng_byte0;
    unsigned char eng_byte1;

    flash_dst=num;
    flash_dst=(num-'0')*32;
    for (j=0; j<32; j++)
    {
        eng_font_img[j]= TraceFont[flash_dst++];
    }

    addr = prt_img_buffer->write_addr ;//*.write_addr=0x0300或0X0900

    if(prt_ctrl_height == 1)
    {
        addr -= 0x0300;                 //减后addr=0x0或0x0600
        prt_img_buffer->prt_addr=addr;  //img地址归位 =0X0或0x600
        prt_img_buffer->height=32;
    }

    buf_pos=draw_x_pos>>3;      // x/8

    for (j=0; j<32 ; j++)
    {
        eng_byte0=eng_font_img[j++];    //low byte
        eng_byte1=eng_font_img[j];   	//heigh byte

        if(num <= '9')
        {
            addr[buf_pos]  = eng_byte0;
            addr[buf_pos+1]= eng_byte1;
            buf_pos+=48;

            addr[buf_pos]  = eng_byte0;
            addr[buf_pos+1]= eng_byte1;
            buf_pos+=48;
        }

        else if(num > '9')
        {
            addr[buf_pos]  = width_h(eng_byte0);
            addr[buf_pos+1]= width_l(eng_byte0);
            addr[buf_pos+2]= width_h(eng_byte1);
            addr[buf_pos+3]= width_l(eng_byte1);
            buf_pos+=48;

            addr[buf_pos]  = width_h(eng_byte0);
            addr[buf_pos+1]= width_l(eng_byte0);
            addr[buf_pos+2]= width_h(eng_byte1);
            addr[buf_pos+3]= width_l(eng_byte1);
            buf_pos+=48;
        }

    }
    if(num > '9')
        draw_x_pos += 32;
    else if(num <= '9')
        draw_x_pos += 16;
}


char get_char(void)
{
    char data;
    ch_exist=0;

    if(prt_read == prt_write)
    {
        prt_read=prt_write=0;
        return 0;
    }
    data=prt_buf[prt_read];
    if(++prt_read >= 1024)
        prt_read=0;

    ch_exist=1;
    return data;
}


void start_prt(void)
{
    unsigned char ch;
    while(1)
    {
       // CommInterpreter(&CommBufUart0);
        //CommInterpreter(&CommBufUart1);
        //CommInterpreter(&CommBufUsb); //吝惫滚傈篮 USB device 荤侩救窃
#ifdef USE_ENET
        //   CommInterpreter(&CommBufEthData[1]);
        //serial_proc(&CommBufEthData[0]);
#endif
        ch=get_char();
        if(ch_exist)
        {
            revchar_proc(ch);
        }
        else return;
    }
}
