#include "SocketConnet.h"
#include "TcpSocketServer.h"


SocketConnet::SocketConnet(QObject *parent)
{

}
SocketConnet::~SocketConnet(void)
{

}

void SocketConnet::Start()
{
    Descriptor = this->socketDescriptor();
    connect(this,SIGNAL(readyRead()),this,SLOT(ReceivedClientData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}
void SocketConnet::ReceivedClientData()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = readAll();
    if(!buffer.isEmpty())
    {
        QString str =  QString::fromLocal8Bit(buffer);
         //qDebug()<< "received :" <<str;
        //数据处理
         emit GetDataFromClient(str, Descriptor);

    }
}

void SocketConnet::slotDisconnected()
{
    qDebug()<< "TcpClientSocket slotDisconnected" << Descriptor;
    emit ClientDisConnected(Descriptor);
}
