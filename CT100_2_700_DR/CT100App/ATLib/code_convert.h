#ifndef CODE_CONVERT___H
#define CODE_CONVERT___H
#include "iconv.h"
#include <string>
//#include <iostream>
//using namespace std;
class code_convert
{
    public:
        int code_converts(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf, size_t outlen);
        int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
        int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen);
    protected:
    private:
};

#endif // CODE_CONVERT_H
