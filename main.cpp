#include <QCoreApplication>
#include "beautifulbing.h"

QString argument(int index){
    QStringList args=QCoreApplication::arguments();

    if(index>=args.length())
        return NULL;

    return args.length()>1?args.at(index):NULL;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /*
     * command:
     * bbcli [image save path] [meta save path](optional)
     *
     * example:
     * bbcli d:\bing\today.jpg d:\bing\meta.ini
     */

    QString imageSavePath(argument(1));

    if(imageSavePath.isEmpty())
        exit(0);

    QString metaSavePath(argument(2));

    BeautifulBing bing;

    QObject::connect(&bing,&BeautifulBing::allDone,
                     &a,&QCoreApplication::quit);

    bing.getMeTodaysImage(imageSavePath,metaSavePath);

    return a.exec();
}
