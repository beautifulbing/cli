#include "imagemeta.h"

#include <QSettings>

ImageMeta::ImageMeta(QObject *parent) : QObject(parent)
{

}

void ImageMeta::set(QString key, QString value)
{
    meta[key]=value;
}

void ImageMeta::store(QString path)
{
    QSettings ini(path,QSettings::IniFormat);

    ini.beginGroup("meta");

    QMapIterator<QString, QString> i(meta);
    while (i.hasNext()) {
        i.next();
        ini.setValue(i.key(),i.value());
    }

    ini.endGroup();
}
