#pragma once

#include <QObject>
#include <functional>
#include <qeventloop.h>
#include <qthread.h>

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

struct MikanNewRssItemInfo {
    Q_GADGET

    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString group MEMBER group)
    Q_PROPERTY(int id MEMBER id)

public:
    QString title;
    QString group;
    int id;

    QString toString() const {
        return QString("{title: %1, group: %2, id: %3}")
            .arg(title).arg(group).arg(id);
    }
};
Q_DECLARE_METATYPE(MikanNewRssItemInfo);

inline uint qHash(const MikanNewRssItemInfo& info) {
    return qHash(info.group) ^ qHash(info.id);
}

inline bool operator==(const MikanNewRssItemInfo& i1, const MikanNewRssItemInfo& i2) {
    return i1.group == i2.group && i1.id == i2.id;
}

class MikanRssReader : public QThread
{
    Q_OBJECT

public:
    MikanRssReader(QObject *parent = nullptr);

    static QString rssLink(int bangumiId);

    static void readRssContent(int bangumiId, const std::function<void(const QMap<QString, QList<MikanTorrentLinkData>>&)>& groupDataHandler, QEventLoop* loop = nullptr);

signals:
    void taskStop(QPrivateSignal);
    void newRssItemFound(const QList<MikanNewRssItemInfo>& info);

private:
    static void parseRssContent(const QByteArray& data, const std::function<void(const QMap<QString, QList<MikanTorrentLinkData>>&)>& groupDataHandler);

protected:
    void run() override;

private:
    bool isRunning = true;
};
