#include "usbUpdate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <QFile>

int udisk(int i)
{
    char cmd[128]={0};
    sprintf(cmd, "mount /dev/sda%d /mnt", i);
    system(cmd);
    if(!access(USBUPDATEPATH,0)){
        return 1;
    }
    return 0;
}

int updateproc(void)
{
    QFile file("/mnt/release/update.tar.bz2");
    if(file.exists())
    {
        system("cp -rf /mnt/release/update.tar.bz2 /root/Application/");
        if (system("tar -xjvf /mnt/release/update.tar.bz2") == 0)
        {
            if (system("/root/Application/update/update.sh") != 0)
            {
                 printf("__________update.sh fail___________\n");
            }
            else
            {
                ;
            }
        }
        else
        {
            printf("----------------------------------------tar -zxvf /mnt/release/update.tar.bz2 fail\n");
        }
    }
    else
    {
        printf("----------------------------------------/mnt/release/update.tar.bz2  file no find\n");
    }
}
int updateproc_tar(void)
{
    QFile file("/root/Application/update.tar.bz2");
    if(file.exists())
    {
        if (system("tar -xjvf /mnt/release/update.tar.bz2") == 0)
        {
            if (system("/root/Application/update/update.sh") != 0)
            {
                 printf("__________update.sh fail___________\n");
            }
            else
            {
                ;
            }
        }
        else
        {
            printf("----------------------------------------tar -zxvf /root/Application/update.tar.bz2 fail\n");
        }
    }
    else
    {
        printf("----------------------------------------/root/Application/update.tar.bz2  file no find\n");
    }
}
int update_usb(void)
{
    int i;
    if(1) printf("----------------------------------------About usb mount\n");
    for(i = 0; i < 9; i ++)
    {
        if(udisk(8-i))
        {
            //执行u盘下制定目录的sh
            printf("----------------------------------------usb mount success\n");
            updateproc();

            break;
        }
    }
    return 0;
}
