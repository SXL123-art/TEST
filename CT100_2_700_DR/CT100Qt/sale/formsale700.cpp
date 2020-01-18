#include "formsale700.h"
#include "ui_formsale700.h"

Formsale700::Formsale700(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formsale700)
{
    ui->setupUi(this);
    //设置背景图片
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.

    qDebug()<<"Q_LINUX_ARM";
    QString qstrLabelSheet_Weight = "QLabel {color:black;font:bold 120pt \"hanwangmingblack\";}";
    QString qstrLabelSheet_Plu = "QLabel {color:yellow;font:bold 90pt \"simhei\";}";
    QString qstrLabelSheet_Describe = "QLabel {color:red;font:50pt \"simhei\";}";
    QString qstrLabelSheet_Bar = "QLabel {color:red;font:45pt \"simhei\";}";
    QString qstrLabelSheet_Time = "QLabel {color:black;font:45pt \"simhei\";}";

    this->ui->label_plu->setAlignment(Qt::AlignLeft);
    this->ui->label_plu->setStyleSheet(qstrLabelSheet_Plu);
    this->ui->label_tare->setStyleSheet(qstrLabelSheet_Weight);
    this->ui->label_weight->setStyleSheet(qstrLabelSheet_Weight);
    this->ui->label_price->setStyleSheet(qstrLabelSheet_Weight);
    this->ui->label_total->setStyleSheet(qstrLabelSheet_Weight);

    this->ui->label_QTime->setStyleSheet(qstrLabelSheet_Time);
    this->ui->label_F1->setStyleSheet(qstrLabelSheet_Bar);
    this->ui->label_F2->setStyleSheet(qstrLabelSheet_Bar);
    this->ui->label_F3->setStyleSheet(qstrLabelSheet_Bar);
    this->ui->label_F4->setStyleSheet(qstrLabelSheet_Bar);

    //版本信息
    this->ui->label_ver->setStyleSheet("QLabel {color:black;font:30pt \"simhei\";}");
    this->ui->label_mode->setStyleSheet("QLabel {color:black;font:40pt \"simhei\";}");

    QPixmap pmSt("./image/43_st_on.png");
    pmSt.scaled(ui->label_stable->size(), Qt::KeepAspectRatio);
    ui->label_stable->setScaledContents(true);
    ui->label_stable->setPixmap(pmSt);

    QPixmap pmNet("./image/43_net_on.png");
    pmNet.scaled(ui->label_net->size(), Qt::KeepAspectRatio);
    ui->label_net->setScaledContents(true);
    ui->label_net->setPixmap(pmNet);

    QPixmap pmZero("./image/43_zero_on.png");
    pmZero.scaled(ui->label_zero->size(), Qt::KeepAspectRatio);
    ui->label_zero->setScaledContents(true);
    ui->label_zero->setPixmap(pmZero);

    ui->label_wifi->setScaledContents(true);
    ui->label_ACDC->setScaledContents(true);

    this->ui->label_describe->setStyleSheet(qstrLabelSheet_Describe);
    this->ui->label_describe->setAlignment(Qt::AlignLeft);

    this->ui->label_cas->setStyleSheet("QLabel {color:red;font:40pt \"simhei\";}");
    this->ui->label_cas->setAlignment(Qt::AlignRight);

    this->ui->label_Name->setStyleSheet("QLabel {color:yellow;font:40pt \"simhei\";}");
    this->ui->label_Name->setAlignment(Qt::AlignRight);

    ui->label_update->hide();
}

Formsale700::~Formsale700()
{
    delete ui;
}

void Formsale700::updateSaleUi(stTabSaleUi*  m_TabSaleUi)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->label_QTime->setText(time.toStdString().c_str());

    ui->label_weight->setText(m_TabSaleUi->weight);
    ui->label_price->setText(m_TabSaleUi->unitprice);
    ui->label_tare->setText(m_TabSaleUi->tare);
    ui->label_total->setText(m_TabSaleUi->totalprice);
    ui->label_plu->setText(m_TabSaleUi->plu);

    this->ui->label_F1->setText(m_TabSaleUi->F1);
    this->ui->label_F2->setText(m_TabSaleUi->F2);
    this->ui->label_F3->setText(m_TabSaleUi->F3);
    this->ui->label_F4->setText(m_TabSaleUi->F4);
    this->ui->label_ver->setText(m_TabSaleUi->ver);
    this->ui->label_Name->setText(m_TabSaleUi->sellerName);
    this->ui->label_cas->setText(m_TabSaleUi->systemName);

    this->ui->label_describe->setText(m_TabSaleUi->message);

    if(m_TabSaleUi->saleMode)
        this->ui->label_mode->setText("计件");
    else
        this->ui->label_mode->setText("计重");

    if(m_TabSaleUi->stable)
        this->ui->label_stable->setVisible(true);
    else
        this->ui->label_stable->setVisible(false);

    if(m_TabSaleUi->net)
        ui->label_net->setVisible(true);
    else
        ui->label_net->setVisible(false);

    if(m_TabSaleUi->zero)
        ui->label_zero->setVisible(true);
    else
        ui->label_zero->setVisible(false);

    //QPixmap logopicurl("./logopicurl.png");

    int ad_value = atoi(m_TabSaleUi->AD);
    if(m_TabSaleUi->ACDC == enDC)
    {

        if(ad_value<=10)
        {
            this->ui->label_ACDC->setStyleSheet("border-image: url(:/image/dc0.png);");

          //  QPixmap dcP1("./image/dc0.png");
          //  ui->label_ACDC->setPixmap(dcP1);
        }
        else if(ad_value <=20)
        {
            this->ui->label_ACDC->setStyleSheet("border-image: url(:/image/dc1.png);");
          //  QPixmap dcP2("./image/dc1.png");
          //  ui->label_ACDC->setPixmap(dcP2);
        }
        else if(ad_value <=40)
        {
              this->ui->label_ACDC->setStyleSheet("border-image: url(:/image/dc2.png);");
          //  QPixmap dcP3("./image/dc2.png");
          //  ui->label_ACDC->setPixmap(dcP3);
        }
        else if(ad_value <=60)
        {
            this->ui->label_ACDC->setStyleSheet("border-image: url(:/image/dc3.png);");
          //  QPixmap dcP("./image/dc3.png");
          //  ui->label_ACDC->setPixmap(dcP);
        }
        else if(ad_value <=80)
        {
            this->ui->label_ACDC->setStyleSheet("border-image: url(:/image/dc4.png);");
         //   QPixmap dcP4("./image/dc4.png");
         //   ui->label_ACDC->setPixmap(dcP4);
        }
        else if(ad_value<=100)
        {
            this->ui->label_ACDC->setStyleSheet("border-image: url(:/image/dc_full.png);");
           // QPixmap dcP5("./image/dc_full.png");
           // ui->label_ACDC->setPixmap(dcP5);
        }
    }
    else if(m_TabSaleUi->ACDC == enAC)
    {
        if(ad_value > 40)
        {
            this->ui->label_ACDC->setStyleSheet("border-image: url(:/image/ac_high.png);");
           // QPixmap dcP6("./image/ac_high.png");
           // ui->label_ACDC->setPixmap(dcP6);
        }
        else
        {
            this->ui->label_ACDC->setStyleSheet("border-image: url(:/image/ac_low.png);");
          //  QPixmap dcP7("./image/ac_low.png");
          //  ui->label_ACDC->setPixmap(dcP7);
        }
    }

    if(m_TabSaleUi->wifi == enWLANT)
    {
        this->ui->label_wifi->setStyleSheet("border-image: url(:/image/wlant.png);");
      //  QPixmap wifiP1("./image/wlant.png");
      //  ui->label_wifi->setPixmap(wifiP1);
    }
    else if(m_TabSaleUi->wifi == enWLANX)
    {
        this->ui->label_wifi->setStyleSheet("border-image: url(:/image/wlanx.png);");
      //  QPixmap wifiP2("./image/wlanx.png");
      //  ui->label_wifi->setPixmap(wifiP2);
    }
    else if(m_TabSaleUi->wifi == enLANT)
    {
        this->ui->label_wifi->setStyleSheet("border-image: url(:/image/lant.png);");
       // QPixmap wifiP3("./image/lant.png");
      //  ui->label_wifi->setPixmap(wifiP3);
    }
    else if(m_TabSaleUi->wifi == enLANX)
    {
        this->ui->label_wifi->setStyleSheet("border-image: url(:/image/lanx.png);");
      //  QPixmap wifiP4("./image/lanx.png");
      //  ui->label_wifi->setPixmap(wifiP4);
    }

    //网络label_Externalweb
    if(m_TabSaleUi->ExternalWeb)
    {
        this->ui->label_Externalweb->setStyleSheet("border-image: url(:/image/On-line-wai.png);");
      //  QPixmap webP1("./image/On-line-wai.png");
      //  ui->label_Externalweb->setPixmap(webP1);
    }
    else
    {
        this->ui->label_Externalweb->setStyleSheet("border-image: url(:/image/Off-line-wai.png);");
      //  QPixmap webP2("./image/Off-line-wai.png");
      //  ui->label_Externalweb->setPixmap(webP2);
    }

    if(strlen(m_TabSaleUi->downloadUrl) > 0)
    {
        ui->label_update->setText("固件更新中...");
        ui->label_update->show();
        qDebug()<<"-------------------downloadUrl:"<<m_TabSaleUi->downloadUrl<<endl;

//        url = QUrl(m_TabSaleUi->downloadUrl);
//        QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
//        request.setUrl(url);
//        reply = accessManager->get(request);//通过发送数据，返回值保存在reply指针里.
//        connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyDownloadFinished(QNetworkReply*)));
    }
    else
    {

        ui->label_update->hide();
    }
//    if(strlen(m_TabSaleUi->picturlUrl) > 0)
//    {
//        system("rm -rf /root/Application/logopicurl.png");
//        printf("-------------------picturlUrl:%s\n",m_TabSaleUi->picturlUrl);
//        this->ui->label_describe->setText("正在下载LOGO图片...");
//        urlLogo = QUrl(m_TabSaleUi->picturlUrl);
//        QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
//        requestLogo.setUrl(urlLogo);
//        replyLogo = accessManager->get(requestLogo);//通过发送数据，返回值保存在reply指针里.
//        connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyDownloadLogoFinished(QNetworkReply*)));
//    }
}

// 响应结束
void Formsale700::replyDownloadFinished(QNetworkReply *reply)
{
    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QFileInfo fileInfo = url.path();
        file = new QFile(fileInfo.fileName());
        file->open(QIODevice::WriteOnly);//只读方式打开文件
        file->write(reply->readAll());
        if (system("tar -xjvf /root/Application/update.tar.bz2") == 0)
        {
            if (system("/root/Application/update/update.sh") != 0)
            {
                 printf("__________update.sh fail___________\n");
            }
        }
    }
    reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;
    ui->label_update->hide();
}
//void Formsale700::replyDownloadLogoFinished(QNetworkReply *reply)
//{
//    //无错误返回
//    if(reply->error() == QNetworkReply::NoError)
//    {
//        QFileInfo fileInfo = urlLogo.path();
//        fileLogo = new QFile(fileInfo.fileName());
//        fileLogo->open(QIODevice::WriteOnly);//只读方式打开文件
//        fileLogo->write(reply->readAll());
//        system("chmod 777 logopicurl.png");
//        this->ui->label_describe->setText("下载LOGO图片成功...");
//    }
//    reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;
//}

void Formsale700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
