#ifndef MY__RING_H
#define MY__RING_H


#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define DEFRINGNUM 1024*5	

#define DEBUG_RING  0

class CMyRing
{
public:
    CMyRing(void)
    {
        m_in = 0;
        m_out = 0;
        m_ValidIn = 0;
        m_ValidOut = 0;
        m_Totalsize = 0;

        m_p = NULL;

        ringinit(DEFRINGNUM);


        pthread_mutex_init(&ring_mutex,NULL);
     };
    CMyRing(int size)
    {
        m_in = 0;
	m_out = 0;
	m_ValidIn = 0;
	m_ValidOut = 0;
	m_Totalsize = 0;

	m_p = NULL;

	ringinit(size);
	pthread_mutex_init(&ring_mutex,NULL); 
        };

	~CMyRing(void)
        {
            delete[] m_p;
            pthread_mutex_destroy(&ring_mutex);
        };

	int ringinit(int size)
        {
            int ret = 0;

            //测试看看可以内存是否够用
            do{
		m_p =  new char[size];

		if(!m_p){	//分配失败						
			size -= 512;
			if(DEBUG_RING) printf("ringbuf malloc failure: [size:%d]!\n",m_Totalsize);
		}else{	//分配成功			
			m_Totalsize = size;			
			break;
		}
            }while(size > 1024);

            if(m_Totalsize == 0){
		ret = -1;
		if(DEBUG_RING) printf("ringbuf malloc failure: [size:%d]!\n",m_Totalsize);
            }else{
		m_ValidIn = m_Totalsize;	
		if(DEBUG_RING) printf("ringbuf mallco ok: [size:%d]!\n",m_Totalsize);
            }

            return ret;
	};

	int ringwrite(unsigned char* s, int size)
        {
            int res = 0;

            pthread_mutex_lock(&ring_mutex);


            if(m_ValidIn < size){
		//剩余空间不够存储		
            }else{
		//
		if(m_in+size<=m_Totalsize){
			//存储空间未到尾部情况
			memcpy(m_p+m_in, s, size);			
		}else{
			//存储空间到尾部情况
			memcpy(m_p+m_in, s, (m_Totalsize-m_in));
			//转到环形缓存首部存储
			memcpy(m_p, s+(m_Totalsize-m_in), size-(m_Totalsize-m_in));
			//m_in += size;
		}		

		m_in += size;		
		if(m_in >= m_Totalsize){
			m_in -= m_Totalsize;
		}

		//读写有效长度设置
		m_ValidIn -= size;
		m_ValidOut+= size;
	
		res = size;
            }
	pthread_mutex_unlock(&ring_mutex);
	return res;
        };

	int ringread(unsigned char* s, int size)
        {
            int res = 0;
            pthread_mutex_lock(&ring_mutex);

            if(m_ValidOut < size){
		//有效数据不够	
            }else{
		if(m_out+size<=m_Totalsize){//存储空间未到尾部情况
			memcpy(s, m_p+m_out, size);			
		}else{						//存储空间到尾部情况
			memcpy(s, m_p+m_out, (m_Totalsize-m_out));

			//转到环形缓存首部读取
			memcpy(s+(m_Totalsize-m_out), m_p, size-(m_Totalsize-m_out));			
		}		

		m_out += size;		
		if(m_out >= m_Totalsize){
			m_out -= m_Totalsize;
		}

		//读写有效长度设置
		m_ValidOut -= size;
		m_ValidIn  += size;

		res = size;
	}

	pthread_mutex_unlock(&ring_mutex);
	return res;
        };

	int getReadValidSize()
        {
		return m_ValidOut;
        };
	int getWriteValidSize()
        {
		return m_ValidIn;
        };

	int getbufsize()
        {
		return m_Totalsize;
        };

	void clean()
        {
		
	pthread_mutex_lock(&ring_mutex);
	m_in = 0;
	m_out = 0;
	m_ValidIn = m_Totalsize;
	m_ValidOut = m_Totalsize - m_ValidIn;

	pthread_mutex_unlock(&ring_mutex);

        };

	char look()
	{
		return *(m_p+m_out);
	};
	
	char look(int step)
	{

            unsigned int find = m_out+step;
            if(find >= m_Totalsize){
                    find -= m_Totalsize;
            }
            return *(m_p+find);
	};

private:
	unsigned int		m_in;			// 写序号
	unsigned int		m_out;			// 读序号
	unsigned int		m_ValidIn;		// 写有效空间长度
	unsigned int		m_ValidOut;		// 读有效空间长度

	char*				m_p;
	unsigned int		m_Totalsize;

	pthread_mutex_t		ring_mutex ;//= PTHREAD_MUTEX_INITIALIZER;
};
#endif

