#ifndef BEAUTIFULBING_H
#define BEAUTIFULBING_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

class BeautifulBing : public QObject
{
    Q_OBJECT

public:
    explicit BeautifulBing(QObject *parent = 0);
    void getAndApplyTodaysImage(const QString imageSavePath);
    void getAndApplyTodaysImage(const QString imageSavePath,const QString metaSavePath);

signals:
    void allDone();

protected slots:
    void httpFinished();
    void imageDownloaded();

protected:
    QTextStream cout;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    QString imageSavePath;
    QString metaSavePath;

    void downloadImage(const QString url);
    void saveMeta(QString title, QString author);
    QRegularExpressionMatch regMatch(QString pattern,QString target);
};

#endif // BEAUTIFULBING_H
