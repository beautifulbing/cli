#include "beautifulbing.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QSettings>
#include "windows.h"

BeautifulBing::BeautifulBing(QObject *parent)
    :QObject(parent)
    ,cout(stdout, QIODevice::WriteOnly)
{

}

void BeautifulBing::getAndApplyTodaysImage(const QString imageSavePath)
{
    cout<<"Visiting Bing's main site..."<<endl;

    this->imageSavePath=imageSavePath;

    QUrl url("http://bing.com/");

    reply=qnam.get(QNetworkRequest(url));

    connect(reply,&QNetworkReply::finished,this,&BeautifulBing::httpFinished);
}

void BeautifulBing::getAndApplyTodaysImage(const QString imageSavePath, const QString metaSavePath)
{
    this->metaSavePath=metaSavePath;

    getAndApplyTodaysImage(imageSavePath);
}

void BeautifulBing::httpFinished()
{
    {//Redirection
        QString redirect=reply->rawHeader("Location");

        //HTTP 301
        if(redirect.length()>0){
            //Redirect to local site
            QUrl url(redirect);
            reply=qnam.get(QNetworkRequest(url));
            connect(reply,&QNetworkReply::finished,this,&BeautifulBing::httpFinished);
            cout<<"Redirected to "<<redirect<<endl;
            return;
        }
    }

    QString res=reply->readAll();

    {//Fetch image url
        QRegularExpressionMatch match=regMatch(
                    "g_img={url: \"(http://.+_\\d+x\\d+\\.jpg)\""
                    ,res);

        if(match.hasMatch()){
            QString picUrl = match.captured(1);

            downloadImage(picUrl);

            cout<<"Image url is "<<picUrl<<endl;
        }
    }

    {//Fetch image meta
        QRegularExpressionMatch match=regMatch(
                    "<a id=\"sh_cp\" class=\"sc_light\" title=\"([^\"]+)\" alt=\""
                    ,res);

        if(match.hasMatch()){
            QString meta = match.captured(1);

            match=regMatch(
                        "^(.+) \\(([^)]+)\\)$"
                        ,meta);

            if(!match.hasMatch()) return;

            QString title=match.captured(1);
            QString author=match.captured(2);

            cout<<"Title: "<<title<<endl;
            cout<<"Author: "<<author<<endl;

            saveMeta(title,author);

            cout<<"Meta has been saved to "<<metaSavePath<<endl;
        }
    }
}

void BeautifulBing::imageDownloaded()
{
    file=new QFile(imageSavePath);

    if(file->open(QIODevice::WriteOnly)){
        file->write(reply->readAll());
    }

    file->close();

    if(file->exists()){
        cout<<"Image has been saved to "<<imageSavePath<<endl;

        //It's very important to be converted into a string
        TCHAR *path = (TCHAR *)imageSavePath.toStdWString().c_str();

        //Invoke WIN32 API to change desktop wallpaper
        SystemParametersInfo(
                    SPI_SETDESKWALLPAPER,
                    0,
                    (PVOID)path,
                    SPIF_UPDATEINIFILE|SPIF_SENDCHANGE);

        emit allDone();
    }

    file->deleteLater();
    file=Q_NULLPTR;
}

void BeautifulBing::downloadImage(const QString url)
{
    reply=qnam.get(QNetworkRequest(QUrl(url)));
    connect(reply,&QNetworkReply::finished,this,&BeautifulBing::imageDownloaded);
}

void BeautifulBing::saveMeta(QString title, QString author)
{
    QSettings meta(metaSavePath,QSettings::IniFormat);

    meta.beginGroup("meta");
    meta.setValue("title",title);
    meta.setValue("author",author);
    meta.endGroup();
}

inline QRegularExpressionMatch BeautifulBing::regMatch(QString pattern,QString target)
{
    QRegularExpression rx(pattern);

    return rx.match(target);
}
