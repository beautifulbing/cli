#include "beautifulbing.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QRegularExpression>
#include "windows.h"

BeautifulBing::BeautifulBing(QObject *parent)
    :QObject(parent)
    ,cout(stdout, QIODevice::WriteOnly)
{

}

void BeautifulBing::getAndApplyTodaysPicture(const QString pathToSave)
{
    cout<<"Visiting Bing's main site..."<<endl;

    this->pathToSave=pathToSave;

    QUrl url("http://bing.com/");

    reply=qnam.get(QNetworkRequest(url));

    connect(reply,&QNetworkReply::finished,this,&BeautifulBing::httpFinished);
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

    QString pattern("g_img={url: \"(http://.+_\\d+x\\d+\\.jpg)\"");
    QRegularExpression rx(pattern);

    QRegularExpressionMatch match=rx.match(res);

    if(match.hasMatch()){
        QString picUrl = match.captured(1);

        downloadPicture(picUrl);

        cout<<"Picture url is "<<picUrl<<endl;
    }
}

void BeautifulBing::pictureDownloaded()
{
    file=new QFile(pathToSave);

    if(file->open(QIODevice::WriteOnly)){
        file->write(reply->readAll());
    }

    file->close();

    if(file->exists()){
        cout<<"Picture has been saved to "<<pathToSave<<endl;

        //It's very important to be converted into a string
        TCHAR *path = (TCHAR *)pathToSave.toStdWString().c_str();

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

void BeautifulBing::downloadPicture(const QString url)
{
    reply=qnam.get(QNetworkRequest(QUrl(url)));
    connect(reply,&QNetworkReply::finished,this,&BeautifulBing::pictureDownloaded);
}
