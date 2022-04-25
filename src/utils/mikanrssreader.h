#pragma once

#include <QObject>
#include <functional>

struct MikanTorrentLinkData {
    Q_GADGET

    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(int size MEMBER size)
    Q_PROPERTY(QString linkUrl MEMBER linkUrl)
    Q_PROPERTY(QString downloadUrl MEMBER downloadUrl)

public:
    QString title;
    int size;
    QString linkUrl;
    QString downloadUrl;

    QString toString() const {
        return QString("{title: %1, size: %2, linkUrl: %3, downloadUrl: %4}")
            .arg(title).arg(size).arg(linkUrl).arg(downloadUrl);
    }
};
Q_DECLARE_METATYPE(MikanTorrentLinkData)

class MikanRssReader : public QObject
{
public:
    MikanRssReader(QObject *parent = nullptr);

    void readRssContent(int bangumiId, const std::function<void(const QMap<QString, QList<MikanTorrentLinkData>>&)>& groupDataHandler);

private:
    void parseRssContent(const QByteArray& data, const std::function<void(const QMap<QString, QList<MikanTorrentLinkData>>&)>& groupDataHandler);

};
