#include <QtCore/QCoreApplication>

#include "unit/machine.h"
#include <QString>
//#include <QWSServer>
#include <QTextCodec>


int main(int argc, char *argv[])
{
    printf("---------------------------CT100App\n");
    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    CMachine *machine = new CMachine();
    machine->run();
    return a.exec();
}
