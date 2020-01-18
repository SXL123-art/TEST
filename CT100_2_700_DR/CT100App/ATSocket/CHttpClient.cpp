#include "CHttpClient.h"
#include <curl/curl.h>
#include <string>
#include <string.h>


CHttpClient::CHttpClient(void) :m_bDebug(false)
{

}

CHttpClient::~CHttpClient(void)
{

}

static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if(itype == CURLINFO_TEXT)
	{
		//printf("[TEXT]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_IN)
	{
		printf("[HEADER_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_OUT)
	{
		printf("[HEADER_OUT]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_IN)
	{
        //printf("[DATA_IN]%s\n", pData);  
	}
	else if(itype == CURLINFO_DATA_OUT)
	{
        //printf("[DATA_OUT]%s\n", pData);  
	}
    
	return 0;
}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str || NULL == buffer )
	{
		return -1;
	}

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    
	return nmemb;
}
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
int CHttpClient::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)  
{  
	SetDebug(true);
    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_POST, 1);  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
    res = curl_easy_perform(curl); 
	if (res != CURLE_OK)  
	{  
		switch(res)  
		{  
		case CURLE_UNSUPPORTED_PROTOCOL:  
			printf("不支持的协议,由URL的头部指定\n");
			break;
		case CURLE_COULDNT_CONNECT:  
			printf("不能连接到remote主机或者代理\n");
			break;
		case CURLE_HTTP_RETURNED_ERROR:  
			printf("http返回错误\\n");
			break;
		case CURLE_READ_ERROR:  
			printf("读本地文件错误\n");
			break;
		default:  
			printf("_________Posts res: %d____________\n", res);
			break;
		}  
	}  
	
    curl_easy_cleanup(curl);  
    return res;  
}  

int CHttpClient::Get(const std::string & strUrl, std::string & strResponse)  
{  
    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    /** 
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作�?
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出�?
    */  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
    res = curl_easy_perform(curl);  
    curl_easy_cleanup(curl);  
    return res;  
}  

int CHttpClient::Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath)  
{  
SetDebug(true);
    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
    char strHeadersTemp[128] = {0};
    struct curl_slist* headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: text/xml; charset=utf-8");
    headers = curl_slist_append(headers, "Accept-Encoding: gzip,deflate");
    headers = curl_slist_append(headers, "User-Agent: Apache-HttpClient/4.1.1 (java 1.5)");
    sprintf(strHeadersTemp,"SOAPAction: \"urn:%s\"",pCaPath);
    headers = curl_slist_append(headers,  strHeadersTemp);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /*curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
    if(NULL == pCaPath)  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);  
    }  
    else  
    {  
        //缺省情况就是PEM，所以� 需设置，另外支持DER  
        //curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);  
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);  
    }  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);*/
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    if(NULL == pCaPath)
    {
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }
    else
    {
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
            curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
    }

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);    // set time out s
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    res = curl_easy_perform(curl);  
	if (res != CURLE_OK)  
	{  
		switch(res)  
		{  
		case CURLE_UNSUPPORTED_PROTOCOL:  
			printf("不支持的协议,由URL的头部指定\n");
			break;
		case CURLE_COULDNT_CONNECT:  
			printf("不能连接到remote主机或者代理\n");
			break;
		case CURLE_HTTP_RETURNED_ERROR:  
			printf("http返回错误\\n");
			break;
		case CURLE_READ_ERROR:  
			printf("读本地文件错误\n");
			break;
		default:  
			printf("_________Posts res: %d____________\n", res);
			break;
		}  
	}  

    curl_easy_cleanup(curl);  
    return res;  
}

int CHttpClient::url2file(const std::string & strUrl, const std::string & savefile)
{
    const char* pagefilename = savefile.c_str();
    FILE* pagefile;

    curl_global_init(CURL_GLOBAL_ALL);
    CURLcode res;
    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }
    if (m_bDebug)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
    }
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);                  //long??????????????????????s????0???????
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);			//??????

    pagefile = fopen(pagefilename, "wb");
    if (pagefile)
    {
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);
         res = curl_easy_perform(curl);
         fclose(pagefile);
    }
    else
    {
        printf("----------------%s:????\n",pagefilename);
        return -1;
    }

    if (res != CURLE_OK)
    {
            switch (res)
            {
            case CURLE_UNSUPPORTED_PROTOCOL:
                    printf("??????,?URL?????\n");
                    break;
            case CURLE_COULDNT_CONNECT:
                    printf("?????remote??????\n");
                    break;
            case CURLE_HTTP_RETURNED_ERROR:
                    printf("http????\\n");
                    break;
            case CURLE_READ_ERROR:
                    printf("???????\n");
                    break;
            default:
                    printf("_________Posts res: %d____________\n", res);
                    break;
            }
    }
    curl_easy_cleanup(curl);
    return res;
}
  
int CHttpClient::Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath)  
{  
//SetDebug(true);
    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
    if(NULL == pCaPath)  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);  
    }  
    else  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);  
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);  
    }  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
    res = curl_easy_perform(curl);  
    curl_easy_cleanup(curl);  
    return res;  
}  

void CHttpClient::SetDebug(bool bDebug)  
{  
    m_bDebug = bDebug;  
}

