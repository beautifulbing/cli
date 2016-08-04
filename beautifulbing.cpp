#include "beautifulbing.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QFileInfo>
#include <QDir>
#include "bingextractor.h"
#include "imagemeta.h"
#include "wallpaper.h"

BeautifulBing::BeautifulBing(QObject *parent)
    :QObject(parent)
    ,cout(stdout, QIODevice::WriteOnly)
{

}

void BeautifulBing::getMeTodaysImage(const QString imageSavePath, const QString metaSavePath)
{
    this->imageSavePath=imageSavePath;
    this->metaSavePath=metaSavePath;

    cout<<"Visiting Bing's main site..."<<endl;

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

    QString html=reply->readAll();
    BingExtractor extractor(html);

    {//Fetch image url
        QString url = extractor.imageUrl();

        downloadImage(url);
    }

    if(!metaSavePath.isEmpty()){//Fetch image meta
        QString title=extractor.imageTitle();
        QString author=extractor.imageAuthor();

        storeMeta(title,author);
    }
}

void BeautifulBing::ensureTheDirectoryExists(QString dirPath)
{
    QDir dir;
    QFileInfo fi(dirPath);

    auto absPath=fi.absolutePath();

    dir.mkdir(absPath);
}

void BeautifulBing::imageDownloaded()
{
    ensureTheDirectoryExists(imageSavePath);

    Wallpaper wallpaper(imageSavePath);

    auto data=reply->readAll();

    if(wallpaper.store(data)){
        cout<<"Image has been saved to "<<imageSavePath<<endl;

        wallpaper.apply();

        emit allDone();
    }
}

void BeautifulBing::downloadImage(const QString url)
{
    cout<<"Image url is "<<url<<endl;

    reply=qnam.get(QNetworkRequest(QUrl(url)));

    connect(reply,&QNetworkReply::finished,
            this,&BeautifulBing::imageDownloaded);
}

void BeautifulBing::storeMeta(QString title, QString author)
{   
    cout<<"Title: "<<title<<endl;
    cout<<"Author: "<<author<<endl;

    ensureTheDirectoryExists(metaSavePath);

    ImageMeta meta;
    meta.set("title",title);
    meta.set("author",author);
    meta.store(metaSavePath);

    cout<<"Meta has been saved to "<<metaSavePath<<endl;
}
