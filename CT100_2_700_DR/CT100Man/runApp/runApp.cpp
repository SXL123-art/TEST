
#include "runApp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define PATH_MAX 	128

int runapp()
{
    printf("-----------------------------update mode:runapp\n");
    system("pkill -SIGKILL CT100App");
    system("pkill -SIGKILL CT100Qt");

    char strName[32] = {0,};
    char strPath[PATH_MAX] = {0,};
    char strAppPath[PATH_MAX] = {0,};

    //这句话得到本程序所有目录，复制到strPath中
    ssize_t count = readlink("/proc/self/exe", strPath, PATH_MAX);

    int i = 0;
    int j=0;
    for (i = count - 1; i >= 0; i--)
    {
        if (strPath[i] == '/'){
            j++;
            if(j==3){
                break;
            }
        }
    }
    int nLen = count - i;
    strncpy(strName, &strPath[i], nLen);
    strPath[i] = 0x0;

    sprintf(strPath,"%s", "/root/Application");//pauly fix it

    pid_t   nPid = -1;
    nPid = fork();  
    printf("CT100Qt process pid :%d\n",nPid );
    if (nPid == 0)
    {
        memset(strAppPath,0x00,PATH_MAX);
        sprintf(strAppPath, "%s/CT100Qt", strPath);
        if (execlp(strAppPath, strAppPath, "-qws", "-nomouse",(char*)0) == -1)
        {
            printf("ct-qt execl Error : %s\n", strAppPath);
            return 0;
        }
    }

    sleep(1);
    nPid = -1;
    nPid = fork();
    printf("CT100App process pid :%d\n",nPid );
    if (nPid == 0)
    {
        memset(strAppPath,0x00,PATH_MAX);
        sprintf(strAppPath, "%s/CT100App", strPath);
        if (execlp(strAppPath, strAppPath,(char*)0) == -1)
        {
            printf("CT100App exe Error : %s\n", strAppPath);
            return 0;
        }
    }
    return 1;
}
