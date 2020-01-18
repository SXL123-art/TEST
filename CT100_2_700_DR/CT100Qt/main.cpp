#include <QtGui/QApplication>
#include "./showdialog.h"
#include <QWSServer>
#include <QTextCodec>
#include <QDebug>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //隐藏鼠标方法有两种，根据QT版本不同，所以一起使用了
    QApplication::setOverrideCursor(Qt::BlankCursor);

    int screenWidth = 0;
    int screenHeight = 0;

    QDesktopWidget *deskWgt = QApplication::desktop();
    if (deskWgt)
    {
        // 获取设备屏幕大小
        QRect screenRect = deskWgt->screenGeometry();
        screenWidth = screenRect.width();
        screenHeight = screenRect.height();
        qDebug() << "screen width:" <<screenWidth << ",height:" << screenHeight;
    }
    else
    {
        screenWidth = 480;
        screenHeight = 272;
    }
//#ifdef DISPLY_700
//    screenWidth = 800;
//    screenHeight = 480;
// #endif
    // 主窗体显示
    showDialog sdg(screenWidth,screenHeight);

    //QWSServer::setCursorVisible(false);
    sdg.setWindowFlags(Qt::FramelessWindowHint);   // 去掉标题栏
    //sdg.setFixedSize(screenWidth, screenHeight);   // 设置主窗体大小
    sdg.show();

    return a.exec();
}
