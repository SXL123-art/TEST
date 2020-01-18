#include <QtCore/QCoreApplication>
#include <stdio.h>
#include "./usbUpdate/usbUpdate.h"
#include "./runApp/runApp.h"
#include "./qtSocketServer/TcpSocketServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    update_usb();
    if(1 != runapp())   //运行APP与QT程序
    {
        updateproc_tar();   //APP和QT失败，就解压以前的升级包
        runapp();       //然后重启应用
    }

    TcpSocketServer *server = new TcpSocketServer();
    //server->listenClient();

    return a.exec();
}
