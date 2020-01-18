#include "ATKey.h"
#include "../ATBeep/ATBeep.h"

Key  scale_key;

int ATKey_init(int KeyType)
{
	return scale_key._Key_init(KeyType);
}

//��ȡ������ͳһ�ӿ�����������¼�����¼��ɡ����ж��Ƿ�ʱ
unsigned char ATKey_getKey(unsigned char * key_value)
{
	int ret = 0;
	
	ret = scale_key.getKey(key_value);

	if (1 == ret)
	{
		ATBeep();
	}

	return ret;
}
bool ATKey_getStates(void)
{
    return scale_key.getStates();
}

