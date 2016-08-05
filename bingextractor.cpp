#include "bingextractor.h"

BingExtractor::BingExtractor(QString html, QObject *parent)
    :QObject(parent)
    ,html(html)
{
    url=fetchImageUrl();
    meta=fetchImageMeta();

    extractMeta();
}

QString BingExtractor::imageUrl() const
{
    return url;
}

QString BingExtractor::imageMeta() const
{
    return meta;
}

QString BingExtractor::fetchImageUrl()
{
    return regexpMatch("g_img={url: \"(http://.+_\\d+x\\d+\\.jpg)\"");
}

QString BingExtractor::fetchImageMeta()
{
    return regexpMatch("<a id=\"sh_cp\" class=\"sc_light\" title=\"([^\"]+)\" alt=\"")
            .replace("&amp;","&");
}

void BingExtractor::extractMeta()
{
    auto match=regexpMatch("^(.+) \\(([^)]+)\\)$"
                           ,meta);

    if(match.hasMatch()){
        metaTitle=match.captured(1);
        metaAuthor=match.captured(2);
    }
}

QString BingExtractor::regexpMatch(const QString &pattern)
{
    QRegularExpressionMatch match=regexpMatch(pattern,html);

    return match.hasMatch()?match.captured(1):NULL;
}

QRegularExpressionMatch BingExtractor::regexpMatch(const QString &pattern, const QString &text)
{
    QRegularExpression rx(pattern);

    QRegularExpressionMatch match=rx.match(text);

    return match;
}

QString BingExtractor::imageAuthor() const
{
    return metaAuthor;
}

QString BingExtractor::imageTitle() const
{
    return metaTitle;
}
