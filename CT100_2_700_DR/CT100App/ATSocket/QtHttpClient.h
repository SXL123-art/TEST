#ifndef QTHTTP_H
#define QTHTTP_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QString>
#include <QUrl>
#include <QTimer>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
class QtHttpClient: public QObject
{
public:
    explicit QtHttpClient(QObject *parent = 0);
    ~QtHttpClient(void);

    int Post(QNetworkRequest httpRequest,const QString PostData,QString *ResposeData);

    int Get(QNetworkRequest httpRequest,QString *ResposeData);

public:

    QNetworkAccessManager* m_pNetworkManager;//网络管理类
    QNetworkReply* m_pNetworkReply; //封装请求返回信息

};

#endif // QTHTTP_H
