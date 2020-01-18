#include "imx28_key.h"
#include "../public/Ring.h"

CMyRing KeyRing(20);


void* theadKey(void* p)

{		
    Key* pKey = (Key*)p;

    if (1)
    {
        printf("ready read serial key!\n");
    }

    while(1)
    {
        usleep(50*1000);
        unsigned char  key;
        int ret = pKey->_Key_getKey(&key);

        if (ret)
        {
            printf("Get Key: %d......\n", key);
#if OPENBEEP
            pKey->m_beep.beep();
#endif
            KeyRing.ringwrite(&key, 1);
        }
    }
}

Key::Key()
{
    m_nKeyType = 0;

   printf("Create Key Thead...\n");

    pthread_t idSerialKey;
    pthread_create(&idSerialKey, NULL, theadKey, this);
}

Key::~Key()
{
	close_Key();
}

int Key::_Key_init(int KeyType)
{
	m_nKeyType = KeyType;
	
	keyfd = open(DEVICE_DEIVERTEST, O_RDWR);
	if(keyfd <0)
	{
		perror("can not open key-device");
		exit(1);
	}
	else
	{
		printf("open key ok!\n");
	}
	return 0;
}

// 小键盘
unsigned char Key::ParseTransmit(int a, int b)
{
    unsigned char ret = 0;

    if( (a == 254) && (b == 15)){ret = _KEY_PLU1;
    }else if( (a == 253) && (b == 15)){	ret = _KEY_PLU2;
    }else if( (a == 251) && (b == 15)){	ret = _KEY_PLU3;
    }else if( (a == 247) && (b == 15)){	ret = _KEY_PLU4;
    }else if( (a == 239) && (b == 15)){	ret = _KEY_PLU5;
    }else if( (a == 254) && (b == 14)){	ret = _KEY_PLU6;
    }else if( (a == 253) && (b == 14)){	ret = _KEY_PLU7;
    }else if( (a == 251) && (b == 14)){	ret = _KEY_PLU8;
    }else if( (a == 247) && (b == 14)){	ret = _KEY_PLU9;
    }else if( (a == 239) && (b == 14)){	ret = _KEY_PLU10;
    }else if( (a == 254) && (b == 13)){	ret = _KEY_PLU11;
    }else if( (a == 253) && (b == 13)){	ret = _KEY_PLU12;
    }else if( (a == 251) && (b == 13)){	ret = _KEY_PLU13;
    }else if( (a == 247) && (b == 13)){	ret = _KEY_PLU14;
    }else if( (a == 239) && (b == 13)){	ret = _KEY_PLU15;
    }else if( (a == 254) && (b == 12)){	ret = _KEY_PLU16;
    }else if( (a == 253) && (b == 12)){	ret = _KEY_PLU17;
    }else if( (a == 251) && (b == 12)){	ret = _KEY_PLU18;
    }else if( (a == 247) && (b == 12)){	ret = _KEY_PLU19;
    }else if( (a == 239) && (b == 12)){	ret = _KEY_PLU20;
    }else if( (a == 254) && (b == 10)){	ret = _KEY_PLU21;
    }else if( (a == 253) && (b == 10)){	ret = _KEY_PLU22;
    }else if( (a == 251) && (b == 10)){	ret = _KEY_PLU23;
    }else if( (a == 247) && (b == 10)){	ret = _KEY_PLU24;
    }else if( (a == 239) && (b == 10)){	ret = _KEY_PLU25;
    }else if( (a == 254) && (b == 11)){	ret = _KEY_PLU26;
    }else if( (a == 253) && (b == 11)){	ret = _KEY_PLU27;
    }else if( (a == 251) && (b == 11)){	ret = _KEY_PLU28;
    }else if( (a == 247) && (b == 11)){	ret = _KEY_PLU29;
    }else if( (a == 239) && (b == 11)){	ret = _KEY_PLU30;

    }else if( (a == 191) && (b == 11)){	ret = _KEY_NUM0;
    }else if( (a == 223) && (b == 10)){	ret = _KEY_NUM1;
    }else if( (a == 191) && (b == 10)){	ret = _KEY_NUM2;
    }else if( (a == 127) && (b == 10)){	ret = _KEY_NUM3;
    }else if( (a == 223) && (b == 12)){	ret = _KEY_NUM4;
    }else if( (a == 191) && (b == 12)){	ret = _KEY_NUM5;
    }else if( (a == 127) && (b == 12)){	ret = _KEY_NUM6;
    }else if( (a == 223) && (b == 13)){	ret = _KEY_NUM7;
    }else if( (a == 191) && (b == 13)){	ret = _KEY_NUM8;
    }else if( (a == 127) && (b == 13)){	ret = _KEY_NUM9;
    }else if( (a == 127) && (b == 11)){ret = _KEY_CLR;
    }else if( (a == 223) && (b == 11)){ret = _KEY_DOT;

    }else if( (a == 223) && (b == 15)){ret = _KEY_F1;
    }else if( (a == 191) && (b == 15)){ret = _KEY_F2;
    }else if( (a == 127) && (b == 15)){ret = _KEY_F3;
    }else if( (a == 251) && (b == 9)){ret = _KEY_F4;
    }else if( (a == 223) && (b == 14)){ret = _KEY_F5;
    }else if( (a == 191) && (b == 14)){ret = _KEY_F6;
    }else if( (a == 127) && (b == 14)){ret = _KEY_F7;
    }else if( (a == 247) && (b == 9)){	ret = _KEY_F8;
    }else if( (a == 247) && (b == 8)){	ret = _KEY_F9;
    }else if( (a == 239) && (b == 9)){	ret = _KEY_F10;
    }else if( (a == 239) && (b == 8)){	ret = _KEY_F11;
    }else if( (a == 223) && (b == 9)){	ret = _KEY_F12;
    }else if( (a == 223) && (b == 8)){	ret = _KEY_F13;
    }else if( (a == 191) && (b == 9)){	ret = _KEY_F14;
    }else if( (a == 191) && (b == 8)){	ret = _KEY_F15;
    }else if( (a == 127) && (b == 9)){	ret = _KEY_F16;

    }else if( (a == 0xff) && (b == 0xff)){ret = KEY_POWER;		//power没有键值
    }
    return ret;
}

// 中键盘
unsigned char Key::ParseMidTransmit(int a, int b)
{
	unsigned char ret = 0;

	if( (a == 253) && (b == 5)){ret = _KEY_PLU1;
	}else if( (a == 253) && (b == 4)){	ret = _KEY_PLU2;
	}else if( (a == 253) && (b == 3)){	ret = _KEY_PLU3;
	}else if( (a == 253) && (b == 2)){	ret = _KEY_PLU4;
	}else if( (a == 253) && (b == 1)){	ret = _KEY_PLU5;
	}else if( (a == 253) && (b == 0)){ret = _KEY_PLU6;
	}else if( (a == 253) && (b == 14)){ret = _KEY_PLU7;
	}else if( (a == 253) && (b == 13)){ret = _KEY_PLU8;
	}else if( (a == 253) && (b == 12)){ret = _KEY_PLU9;
	}else if( (a == 253) && (b == 11)){ret = _KEY_PLU10;
	}else if( (a == 253) && (b == 10)){	ret = _KEY_PLU11;
	}else if( (a == 253) && (b == 9)){	ret = _KEY_PLU12;
	
	}else if( (a == 251) && (b == 5)){	ret = _KEY_PLU13;
	}else if( (a == 251) && (b == 4)){	ret = _KEY_PLU14;
	}else if( (a == 251) && (b == 3)){ret = _KEY_PLU15;
	}else if( (a == 251) && (b == 2)){ret = _KEY_PLU16;
	}else if( (a == 251) && (b == 1)){ret = _KEY_PLU17;
	}else if( (a == 251) && (b == 0)){ret = _KEY_PLU18;
	}else if( (a == 251) && (b == 14)){ret = _KEY_PLU19;
	}else if( (a == 251) && (b == 13)){	ret = _KEY_PLU20;
	}else if( (a == 251) && (b == 12)){	ret = _KEY_PLU21;
	}else if( (a == 251) && (b == 11)){	ret = _KEY_PLU22;
	}else if( (a == 251) && (b == 10)){	ret = _KEY_PLU23;
	}else if( (a == 251) && (b == 9)){ret = _KEY_PLU24;
	
	}else if( (a == 247) && (b == 5)){ret = _KEY_PLU25;
	}else if( (a == 247) && (b == 4)){ret = _KEY_PLU26;
	}else if( (a == 247) && (b == 3)){ret = _KEY_PLU27;
	}else if( (a == 247) && (b == 2)){ret = _KEY_PLU28;
	}else if( (a == 247) && (b == 1)){	ret = _KEY_PLU29;
	}else if( (a == 247) && (b == 0)){	ret = _KEY_PLU30;
	}else if( (a == 247) && (b == 14)){	ret = _KEY_PLU31;
	}else if( (a == 247) && (b == 13)){	ret = _KEY_PLU32;
	}else if( (a == 247) && (b == 12)){ret = _KEY_PLU33;
	}else if( (a == 247) && (b == 11)){ret = _KEY_PLU34;
	}else if( (a == 247) && (b == 10)){ret = _KEY_PLU35;
	}else if( (a == 247) && (b == 9)){ret = _KEY_PLU36;
	
	}else if( (a == 239) && (b == 5)){	ret = _KEY_PLU37;
	}else if( (a == 239) && (b == 4)){	ret = _KEY_PLU38;
	}else if( (a == 239) && (b == 3)){	ret = _KEY_PLU39;
	}else if( (a == 239) && (b == 2)){	ret = _KEY_PLU40;
	}else if( (a == 239) && (b == 1)){	ret = _KEY_PLU41;
	}else if( (a == 239) && (b == 0)){ret = _KEY_PLU42;
	}else if( (a == 239) && (b == 14)){ret = _KEY_PLU43;
	}else if( (a == 239) && (b == 13)){ret = _KEY_PLU44;
	}else if( (a == 239) && (b == 12)){ret = _KEY_PLU45;
	}else if( (a == 239) && (b == 11)){	ret = _KEY_PLU46;
	}else if( (a == 239) && (b == 10)){	ret = _KEY_PLU47;
	}else if( (a == 239) && (b == 9)){	ret = _KEY_PLU48;
	
	}else if( (a == 223) && (b == 5)){	ret = _KEY_PLU49;
	}else if( (a == 223) && (b == 4)){	ret = _KEY_PLU50;
	}else if( (a == 223) && (b == 3)){ret = _KEY_PLU51;
	}else if( (a == 223) && (b == 2)){ret = _KEY_PLU52;
	}else if( (a == 223) && (b == 1)){ret = _KEY_PLU53;
	}else if( (a == 223) && (b == 0)){ret = _KEY_PLU54;
	}else if( (a == 223) && (b == 14)){ret = _KEY_PLU55;
	}else if( (a == 223) && (b == 13)){	ret = _KEY_PLU56;
	}else if( (a == 223) && (b == 12)){	ret = _KEY_PLU57;
	}else if( (a == 223) && (b == 11)){	ret = _KEY_PLU58;
	}else if( (a == 223) && (b == 10)){	ret = _KEY_PLU59;
	}else if( (a == 223) && (b == 9)){ret = _KEY_PLU60;
	
	}else if( (a == 127) && (b == 3)){	ret = _KEY_NUM0;
	}else if( (a == 239) && (b == 8)){ret = _KEY_NUM1;
	}else if( (a == 127) && (b == 4)){	ret = _KEY_NUM2;
	}else if( (a == 127) && (b == 1)){	ret = _KEY_NUM3;
	}else if( (a == 247) && (b == 8)){ret = _KEY_NUM4;
	}else if( (a == 127) && (b == 5)){	ret = _KEY_NUM5;
	}else if( (a == 127) && (b == 2)){	ret = _KEY_NUM6;
	}else if( (a == 251) && (b == 8)){ret = _KEY_NUM7;
	}else if( (a == 191) && (b == 3)){	ret = _KEY_NUM8;
	}else if( (a == 191) && (b == 0)){	ret = _KEY_NUM9;
	}else if( (a == 127) && (b == 0)){ret = _KEY_DOT;
	}else if( (a == 223) && (b == 8)){	ret = _KEY_CLR;
	
	}else if( (a == 191) && (b == 8)){	ret = _KEY_F1;
	}else if( (a == 191) && (b == 5)){	ret = _KEY_F2;
	}else if( (a == 191) && (b == 2)){ret = _KEY_F3;
	}else if( (a == 191) && (b == 14)){ret = _KEY_F4;
	
	}else if( (a == 253) && (b == 8)){ret = _KEY_F5;
	}else if( (a == 191) && (b == 4)){ret = _KEY_F6;
	}else if( (a == 191) && (b == 1)){	ret = _KEY_F7;
	}else if( (a == 191) && (b == 13)){	ret = _KEY_F8;
	}else if( (a == 191) && (b == 11)){ret = _KEY_F9;
	}else if( (a == 191) && (b == 12)){ret = _KEY_F10;	
	}else if( (a == 191) && (b == 10)){ret = _KEY_F11;
	}else if( (a == 127) && (b == 14)){ret = _KEY_F12;
	}else if( (a == 127) && (b == 11)){	ret = _KEY_F13;
	}else if( (a == 127) && (b == 13)){	ret = _KEY_F14;
	}else if( (a == 127) && (b == 10)){ret = _KEY_F15;
	}else if( (a == 127) && (b == 12)){ret = _KEY_F16;
	}
	return ret;
}

int Key::_Key_getKey(unsigned char * key)
{
    *key = 0;

    read(keyfd, &m_key_Value, sizeof(unsigned int));
    if(m_key_Value != 0)
    {
        int left = (m_key_Value&0x0000ff00)>>8;
        int right =(m_key_Value & 0x000000ff);
        printf("------------------------------------left:%d,right:%d\n", left,right);
        if (m_nKeyType == 2)
        {
                *key = ParseMidTransmit(left, right);
        }
        else
        {
                // 默认小键盘
                *key = ParseTransmit(left, right);
        }
        return 1;
    }
    return 0;
}

int Key::getKey(unsigned char * key)
{
	int ret=0;
	int size = KeyRing.getReadValidSize();
       // printf("getkey size = %d\n",size);
	if(size>0){
		KeyRing.ringread(key, 1);
		ret= 1;
		//printf("test key read!\n");	
	}
	return ret;
}
bool Key::getStates(void)
{
        int size = KeyRing.getReadValidSize();
        if(size>0){
                return true;
        }
        return false;
}
void Key::close_Key()
{
	close(keyfd);
}

int Key::get_cal_key(void)
{
	unsigned char calkey;
	
	ioctl(keyfd, KEY_IO_CAL_KEY, &calkey);

	//printf("--------calkey: %d\n", calkey);
	if (calkey)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

