#include <QCoreApplication>
#include "beautifulbing.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString imageSavePath,metaSavePath;

    /*
     * command:
     * bbcli [savepath]
     *
     * example:
     * bbcli d:\bing\today.jpg
     */
    {
        QStringList *args=new QStringList(QCoreApplication::arguments());

        if(args->length()<2) exit(0);

        imageSavePath=args->at(1);

        if(args->length()>=3)
            metaSavePath=args->at(2);

        delete args;
        args=Q_NULLPTR;
    }

    BeautifulBing bing;
    QObject::connect(&bing,&BeautifulBing::allDone,&a,&QCoreApplication::quit);

    if(metaSavePath.isEmpty())
        bing.getAndApplyTodaysImage(imageSavePath);
    else
        bing.getAndApplyTodaysImage(imageSavePath,metaSavePath);

    return a.exec();
}
