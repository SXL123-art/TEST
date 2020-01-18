#include <QEventLoop>

#include "QtHttpClient.h"
#define TIME_OUT (10*1000)

QtHttpClient::QtHttpClient(QObject *parent) :
    QObject(parent)
{
    m_pNetworkManager = new QNetworkAccessManager(this);
    m_pNetworkReply = NULL;
}

QtHttpClient::~QtHttpClient(void)
{
    delete m_pNetworkManager;
}

int QtHttpClient::Post(QNetworkRequest httpRequest,const QString PostData,QString *ResposeData)
{
    m_pNetworkReply = m_pNetworkManager->post(httpRequest,PostData.toLatin1());

    QTimer timer;//翻遍了官方文档 没有设置超时的接口，采用定时器处理
    timer.setInterval(TIME_OUT);  // 设置超时时间
    timer.setSingleShot(true);// 单次触发
    QEventLoop loop;
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(m_pNetworkReply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start();
    loop.exec();

    int nHttpCode = 0;
    if(timer.isActive())
    {
        timer.stop();
        QNetworkReply::NetworkError error = m_pNetworkReply->error();
        QByteArray bytes;
        switch (error)
        {
            case QNetworkReply::NoError:
                bytes = m_pNetworkReply->readAll();
                *ResposeData = QString::fromUtf8(bytes);
                nHttpCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                break;
            case QNetworkReply::ConnectionRefusedError:
                qDebug() << "远程服务器拒绝连接";
                break;
            case QNetworkReply::HostNotFoundError:
                qDebug() << "远程主机名未找到（无效主机名）";
                break;
            case QNetworkReply::RemoteHostClosedError:
                qDebug() << "远程主机关闭了连接";
                break;
            case QNetworkReply::TimeoutError:
                qDebug() << "连接超时错误";
                break;
            deafult:
                qDebug() << "未知错误";
                break;
        }
    }
    else
    {
        disconnect(m_pNetworkReply,SIGNAL(timeout()), &loop, SLOT(quit()));
        m_pNetworkReply->abort();
        qDebug() << "QTimer 连接超时错误";
    }

    m_pNetworkReply->deleteLater();
    return nHttpCode;
}

int QtHttpClient::Get(QNetworkRequest httpRequest,QString *ResposeData)
{
    m_pNetworkReply = m_pNetworkManager->get(httpRequest);

    QTimer timer;//翻遍了官方文档 没有设置超时的接口，采用定时器处理
    timer.setInterval(TIME_OUT);  // 设置超时时间
    timer.setSingleShot(true);// 单次触发
    QEventLoop loop;
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(m_pNetworkReply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(TIME_OUT);
    loop.exec();

    int nHttpCode = 0;
    if(timer.isActive())
    {
        timer.stop();
        QNetworkReply::NetworkError error = m_pNetworkReply->error();
        QByteArray bytes;
        switch (error)
        {
            case QNetworkReply::NoError:
                bytes = m_pNetworkReply->readAll();
                *ResposeData = QString::fromUtf8(bytes);
                nHttpCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                break;
            case QNetworkReply::ConnectionRefusedError:
                qDebug() << "远程服务器拒绝连接";
                break;
            case QNetworkReply::HostNotFoundError:
                qDebug() << "远程主机名未找到（无效主机名）";
                break;
            case QNetworkReply::RemoteHostClosedError:
                qDebug() << "远程主机关闭了连接";
                break;
            case QNetworkReply::TimeoutError:
                qDebug() << "连接超时错误";
                break;
            deafult:
                qDebug() << "未知错误";
                break;
        }
    }
    else
    {
        disconnect(m_pNetworkReply,SIGNAL(timeout()), &loop, SLOT(quit()));
        m_pNetworkReply->abort();
        qDebug() << "QTimer 连接超时错误";
    }

    m_pNetworkReply->deleteLater();
    return nHttpCode;
}
