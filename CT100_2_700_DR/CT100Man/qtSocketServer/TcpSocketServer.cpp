#include "TcpSocketServer.h"
#include <QFile>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

TcpSocketServer::TcpSocketServer()
{
    if(!listen(QHostAddress::Any, 3230))
    {
        qDebug()<< "Listen Fail :";
    }
    qDebug()<< "-------------------------------------------CT100Man TcpSocketServer listen ";
}
TcpSocketServer::~TcpSocketServer(void)
{

}

void TcpSocketServer::incomingConnection(int socketDescriptor)
{
    //qDebug()<< "incomingConnection socketDescriptor " << socketDescriptor;
    SocketConnet *tcpSocket = new SocketConnet(this);
    tcpSocket->setSocketDescriptor(socketDescriptor);
    tcpSocket->Start();

    connect(tcpSocket,SIGNAL(GetDataFromClient(QString ,int)),this,SLOT(sendDataToClient(QString ,int )));
    connect(tcpSocket,SIGNAL(ClientDisConnected(int)),this,SLOT(slot_Disconnected(int)));

    tcpSocketConnetList.append(tcpSocket);
}

void TcpSocketServer::sendDataToClient(QString Data,int descriptor)
{
    //可对数据进行解析
    int count = tcpSocketConnetList.count();
    //qDebug()<< "sendDataToClient descriptor " << descriptor << count;
     for(int i=0; i<count; i++)
     {
        QTcpSocket *item = tcpSocketConnetList.at(i);
        if(item->socketDescriptor()== descriptor)
        {
            qDebug()<<"data:" << Data;
            QString sendData;
            if(Data == "UpdataScaleProgram")
            {
                //判断升级包是否存在，不存在报错，存在解压拷贝，删除升级包，重启应用程序
                QFile file("/root/Application/update.tar.bz2");
                if(file.exists())
                {
                    //如果文件存在，就解压文件执行脚本完成升级
                    if (system("tar -xjvf /root/Application/update.tar.bz2") == 0)
                    {
                        printf("__________tar jxvf update.tar.bz2 success___________\n");
                        if (system("/root/Application/update/update.sh") != 0)
                        {
                            printf("__________update.sh fail___________\n");
                            sendData="1,update.sh file exe fail";
                        }
                        else
                        {
                            //system("rm -rf /root/Application/update/");
                            //system("rm -rf /root/Application/update.tar.bz2");
                        }
                    }
                    else{
                        sendData="1,tar -zxvf /root/Application/update.tar.bz2 fail";
                        qDebug()<<sendData;
                    }
                }
                else
                {
                    sendData="1,/root/Application/update.tar.bz2 file no find";
                    qDebug()<<sendData;
                }
                if(item->write(sendData.toLocal8Bit()) != sendData.toLocal8Bit().length())
                {
                    qDebug()<< "sendDataToClient write error " ;
                }
                else{
                    qDebug()<< "sendDataToClient write success " ;
                }
                break;
            }
            continue;
        }
     }
}
//客户端断开连接时删除
void TcpSocketServer::slot_Disconnected(int descriptor)
{
    int count = tcpSocketConnetList.count();
    //qDebug()<< "TcpSocketServer slot_Disconnected " << descriptor << count;
    for(int i=0; i<count; i++)
    {
        SocketConnet *item = tcpSocketConnetList.at(i);
        //qDebug()<< i<< item->socketDescriptor()<<"TcpSocketServer slot_Disconnected " << descriptor << count;
        if(item->socketDescriptor()== descriptor)
        {
            qDebug()<< "TcpSocketServer slot_Disconnected " << descriptor << count;
            tcpSocketConnetList.removeAt(i);
            return;
        }
    }
    //qDebug()<< "TcpSocketServer slot_Disconnected " << descriptor << count;
    return;
}
