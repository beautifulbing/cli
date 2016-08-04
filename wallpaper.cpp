#include "wallpaper.h"

#include <QFile>
#include "windows.h"

Wallpaper::Wallpaper(QString filepath, QObject *parent)
    :QObject(parent)
    ,filepath(filepath)
{

}

void Wallpaper::apply()
{
    //It's very important to be converted into a string
    TCHAR *path = (TCHAR *)filepath.toStdWString().c_str();

    //Invoke WIN32 API to change desktop wallpaper
    SystemParametersInfo(
                SPI_SETDESKWALLPAPER,
                0,
                (PVOID)path,
                SPIF_UPDATEINIFILE|SPIF_SENDCHANGE);
}

bool Wallpaper::store(QByteArray data)
{
    QFile file(filepath);
    bool flag=false;

    if(file.open(QIODevice::WriteOnly)){
        flag=file.write(data) != -1;
    }

    file.close();

    return flag;
}
