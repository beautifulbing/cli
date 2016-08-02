#include <QCoreApplication>
#include "beautifulbing.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString saveTo;

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

        saveTo=args->at(1);

        delete args;
        args=Q_NULLPTR;
    }

    BeautifulBing bing;
    QObject::connect(&bing,&BeautifulBing::allDone,&a,&QCoreApplication::quit);
    bing.getAndApplyTodaysPicture(saveTo);

    return a.exec();
}
