#ifndef BEAUTIFULBING_H
#define BEAUTIFULBING_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextStream>

class BeautifulBing : public QObject
{
    Q_OBJECT

public:
    explicit BeautifulBing(QObject *parent = 0);

    void getMeTodaysImage(const QString imageSavePath,const QString metaSavePath=NULL);

signals:
    void allDone();

protected slots:
    void httpFinished();
    void imageDownloaded();

protected:
    //Func
    void downloadImage(const QString url);
    void storeMeta(QString title, QString author);

    //Helper
    void ensureTheDirectoryExists(QString dirPath);

    QTextStream cout;

    QNetworkAccessManager qnam;
    QNetworkReply *reply;

    QString imageSavePath;
    QString metaSavePath;
};

#endif // BEAUTIFULBING_H
