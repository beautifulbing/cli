#ifndef BINGEXTRACTOR_H
#define BINGEXTRACTOR_H

#include <QObject>
#include <QRegularExpression>

class BingExtractor : public QObject
{
    Q_OBJECT
public:
    explicit BingExtractor(QString html, QObject *parent = 0);

    QString imageUrl() const;
    QString imageMeta() const;
    QString imageTitle() const;
    QString imageAuthor() const;

signals:

public slots:

protected:
    QString fetchImageUrl();
    QString fetchImageMeta();
    void extractMeta();
    QString regexpMatch(QString pattern);
    QRegularExpressionMatch regexpMatch(QString pattern,QString text);

    QString html;
    QString url;
    QString meta;
    QString metaTitle;
    QString metaAuthor;
};

#endif // BINGEXTRACTOR_H
