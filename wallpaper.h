#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QObject>

class Wallpaper : public QObject
{
    Q_OBJECT

public:
    explicit Wallpaper(QString filepath, QObject *parent = 0);

    bool store(const QByteArray &data);
    void apply();

signals:

public slots:

protected:
    QString filepath;
};

#endif // WALLPAPER_H
