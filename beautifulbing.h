#ifndef BEAUTIFULBING_H
#define BEAUTIFULBING_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTextStream>

class BeautifulBing : public QObject
{
    Q_OBJECT

public:
    explicit BeautifulBing(QObject *parent = 0);
    void getAndApplyTodaysPicture(const QString pathToSave);

signals:
    void allDone();

protected slots:
    void httpFinished();
    void pictureDownloaded();

protected:
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    QString pathToSave;
    QTextStream cout;

    void downloadPicture(const QString url);
};

#endif // BEAUTIFULBING_H
