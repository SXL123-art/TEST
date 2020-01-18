#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QDebug>

class SocketConnet : public QTcpSocket
{
    Q_OBJECT
public:
    SocketConnet(QObject *parent=0);
    ~SocketConnet(void);
    void Start();

signals:
    void GetDataFromClient(QString ,int);
    void ClientDisConnected(int);

protected slots:
    void ReceivedClientData();
    void slotDisconnected();
private:
    int Descriptor;
};

#endif // TCPCLIENTSOCKET_H
