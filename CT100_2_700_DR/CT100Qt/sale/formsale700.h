#ifndef FORMSALE700_H
#define FORMSALE700_H

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "../public/typeTabUi.h"

namespace Ui {
    class Formsale700;
}

class Formsale700 : public QWidget
{
    Q_OBJECT

public:
    explicit Formsale700(QWidget *parent = 0);
    ~Formsale700();
    void updateSaleUi(stTabSaleUi*  m_TabSaleUi);

private:
    Ui::Formsale700 *ui;
    QNetworkRequest request;
    QNetworkReply *reply;
    QFile *file;
    QUrl url;
protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void replyDownloadFinished(QNetworkReply *reply);
    //void replyDownloadLogoFinished(QNetworkReply *reply);
};

#endif // FORMSALE700_H
