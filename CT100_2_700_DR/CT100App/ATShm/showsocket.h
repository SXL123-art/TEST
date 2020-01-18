#ifndef SHOWSOCKET_H
#define SHOWSOCKET_H
#include "../../Public/typeTabUi.h"
#include "../public/typeShow.h"
//#include "../unit/shareInfo.h"
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

class showSocket
{
public:
     showSocket(QString ip);

    ~showSocket(void);

    void updateSale(stTabSaleUi* p);
    void updatePay(stTabPayUi* p);

    int	m_socket;

    bool connected;

    pthread_mutex_t	mutex;

    QString IPAddress;

};

#endif // SHOWSOCKET_H
