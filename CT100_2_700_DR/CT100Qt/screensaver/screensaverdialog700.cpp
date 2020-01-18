#include "screensaverdialog700.h"
#include "ui_screensaverdialog700.h"
#include <QFile>

ScreenSaverDialog700::ScreenSaverDialog700(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScreenSaverDialog700)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);   // 去掉标题栏
    ui->label_log->setScaledContents(true);
}

ScreenSaverDialog700::~ScreenSaverDialog700()
{
    delete ui;
}

void ScreenSaverDialog700::updateScreenSaverDialog700Ui(stScreenCamQT*  m_TabScreenCamUi)
{
    char imgName[64] = {0};
    switch(m_TabScreenCamUi->cmd)
    {
        case MSG_MESSAGE_REQ:
            if(m_TabScreenCamUi->screen_cam == 0)
            {
                sprintf(imgName,"./ScreenSaver/photo%d.png",m_TabScreenCamUi->index);
                QFile DBfile(imgName);
                if(DBfile.exists())
                {
                    QPixmap pmimg;
                    pmimg.load(imgName);
                    ui->label_log->setPixmap((pmimg));
                    ui->label_log->show();
                }
            }
            else if(m_TabScreenCamUi->screen_cam == 1)
            {
                sprintf(imgName,"%s","./photo/CameraTest.jpg");
                QFile DBfile(imgName);
                if(DBfile.exists())
                {
                    usleep(300*1000);
                    //printf("................qt........CameraTest.jpg\n");
                    QPixmap pmimg;
                    pmimg.load(imgName);
                    ui->label_log->setPixmap((pmimg));
                    ui->label_log->show();
                }
            }

            this->show();
            break;
        case MSG_MESSAGE_OK:
            this->hide();
            break;
        default:
            break;
    }
}

void ScreenSaverDialog700::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

