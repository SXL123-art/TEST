#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#include <QObject>
#include <QSocketNotifier>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include "SocketConnet.h"

class TcpSocketServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpSocketServer();

    ~TcpSocketServer(void);

protected:
    //函数重载
    void incomingConnection(int socketDescriptor);

private slots:
    void sendDataToClient(QString SendData,int descriptor);
    void slot_Disconnected(int descriptor);

public:
    QList<SocketConnet*> tcpSocketConnetList;
};
#endif // TCPSOCKETSERVER_H
