#ifndef IMAGEMETA_H
#define IMAGEMETA_H

#include <QMap>
#include <QObject>

class ImageMeta : public QObject
{
    Q_OBJECT
public:
    explicit ImageMeta(QObject *parent = 0);

    void set(const QString &key,const QString &value);
    void store(const QString &path);
signals:

public slots:

protected:
    QMap<QString,QString> meta;
};

#endif // IMAGEMETA_H
