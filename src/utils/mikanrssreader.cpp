#include "mikanrssreader.h"

#include <QApplication>

#include "HttpClient.h"
#include "dao.h"

#include <qxmlstream.h>
#include <qdebug.h>
#include <qtimer.h>

#include "databasemodels/subscribemodel.h"
#include "databasemodels/settingtbmodel.h"

MikanRssReader::MikanRssReader(QObject *parent)
    : QThread(parent)
{
    qRegisterMetaType<QList<MikanNewRssItemInfo>>();
    connect(qApp, &QGuiApplication::aboutToQuit, this, [&] {
        isRunning = false;
        taskStop(QPrivateSignal());
    });
}

#define BANGUMI_RSS_URL QString("https://mikanani.me/RSS/Bangumi?bangumiId=%1")

QString MikanRssReader::rssLink(int bangumiId) {
    return BANGUMI_RSS_URL.arg(bangumiId);
}

void MikanRssReader::readRssContent(int bangumiId, const std::function<void(const QMap<QString, QList<MikanTorrentLinkData>>&)>& groupDataHandler, QEventLoop* loop) {

    const auto& onFailed = [=](const QByteArray& reason) {
        qDebug() << reason;
        groupDataHandler({});
    };

    const auto& onSuccess = [=](const QByteArray& data) {
        parseRssContent(data, groupDataHandler);
    };

    const auto& onFinished = [=] {
        if (loop != nullptr) {
            loop->quit();
        }
    };

    if (loop != nullptr) {
        AeaQt::HttpClient client;
        client.get(rssLink(bangumiId))
            .onFailed(onFailed)
            .onSuccess(onSuccess)
            .onFinished(onFinished)
            .exec();

        loop->exec();
    } else {
        static AeaQt::HttpClient client;
        client.get(rssLink(bangumiId))
            .onFailed(onFailed)
            .onSuccess(onSuccess)
            .onFinished(onFinished)
            .exec();
    }
}

bool miKanTorrentLinkDataCompare(const MikanTorrentLinkData& left, const MikanTorrentLinkData& right) {
    return left.title.toLower() < right.title.toLower();
}

void MikanRssReader::parseRssContent(const QByteArray& data, const std::function<void(const QMap<QString, QList<MikanTorrentLinkData>>&)>& groupDataHandler) {
    QXmlStreamReader reader(data);

    QList<MikanTorrentLinkData> rssData;
    MikanTorrentLinkData currentLinkData;
    bool itemBegin = false;

    while (!reader.atEnd()) {
        auto token = reader.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == "item") {
                itemBegin = true;
                currentLinkData = MikanTorrentLinkData();
            }
            if (itemBegin) {
                if (reader.name() == "description") {
                    currentLinkData.title = reader.readElementText();
                }
                if (reader.name() == "link") {
                    currentLinkData.linkUrl = reader.readElementText();
                }
                if (reader.name() == "contentLength") {
                    currentLinkData.size = reader.readElementText().toInt();
                }
                if (reader.name() == "enclosure") {
                    currentLinkData.downloadUrl = reader.attributes().value("url").toString();
                }
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (reader.name() == "item") {
                itemBegin = false;
                rssData << currentLinkData;
            }
        }
    }

    /*
    for (const auto& d : rssData) {
        qDebug() << d.toString();
    }
    */
    qDebug() << "read rss data size:" << rssData.size();

    qSort(rssData.begin(), rssData.end(), miKanTorrentLinkDataCompare);

    QMap<QString, QList<MikanTorrentLinkData>> groupLinkData;
    for (const auto& d: rssData) {
        QString groupName;
        do {
            auto title = d.title.trimmed();
            if (title.at(0) == 8203) {
                title = title.mid(1);
            }
            if (title.startsWith('[')) {
                auto right = title.indexOf(']');
                if (right != -1) {
                    groupName = title.mid(1, right - 1);
                    break;
                }
            }
            if (title.startsWith(QStringLiteral("¡¾"))) {
                auto right = title.indexOf(QStringLiteral("¡¿"));
                if (right != -1) {
                    groupName = title.mid(1, right - 1);
                    break;
                }
            }
            groupName = QStringLiteral("ÆäËû");
        } while (false);

        if (!groupLinkData.contains(groupName)) {
            groupLinkData.insert(groupName, QList<MikanTorrentLinkData>());
        }
        groupLinkData[groupName] << d;
    }

    /*
    qDebug() << "grouped torrent link data:";
    for (const auto& key: groupLinkData.keys()) {
        qDebug() << "group:" << key;
        for (const auto& i : groupLinkData[key]) {
            qDebug() << i.toString();
        }
    }
    */
    qDebug() << "grouped size:" << groupLinkData.size();

    groupDataHandler(groupLinkData);
}

void MikanRssReader::run() {
    QEventLoop loop;
    connect(this, &MikanRssReader::taskStop, &loop, &QEventLoop::quit);

    auto syncWaitTimer = new QTimer(&loop);
    syncWaitTimer->setSingleShot(true);
    connect(syncWaitTimer, &QTimer::timeout, &loop, &QEventLoop::quit);

    while (isRunning) {

        QSet<MikanNewRssItemInfo> newRssItems;

        auto allSubscribeTarget = SubScribeGroupsModel::getAllSubscribeTarget();
        for (const auto& target: allSubscribeTarget) {

            QMap<QString, QList<MikanTorrentLinkData>> currentSubscribeData;
            readRssContent(target.getBangumiId(), [&] (const QMap<QString, QList<MikanTorrentLinkData>>& data) {
                currentSubscribeData = data;
            }, &loop);
            if (!isRunning) break;

            for (const auto& key: currentSubscribeData.keys()) {

                auto currentGroupExistItems = SubScribeGroupsModel::getAllSubscribeGroupItems(target.getBangumiId(), key);
                QSet<QString> existSourceLinkList;
                for (const auto& item: currentGroupExistItems) {
                    existSourceLinkList.insert(item.getSourceLink());
                }

                const auto& newSubscribeData = currentSubscribeData[key];
                QSet<QString> newSourceLinks;
                for (const auto& data: newSubscribeData) {
                    if (!existSourceLinkList.contains(data.downloadUrl)) {
                        newSourceLinks.insert(data.downloadUrl);
                    }
                }

                if (!newSourceLinks.isEmpty()) {
                    int groupId = SubScribeGroupsModel::getGroup(target.getId(), key).getId();
                    if (groupId != -1) {
                        SubScribeGroupsModel::insertSubscribeGroupItems(groupId, newSourceLinks.values());
                        newRssItems.insert(MikanNewRssItemInfo{ target.getTitle(), key, target.getId() });
                    }
                }
            }
        }

        if (!newRssItems.isEmpty()) {
            qDebug() << "find new Rss items:";
            for (const auto& item: newRssItems) {
                qDebug() << item.toString();
            }
            emit newRssItemFound(newRssItems.values());

            if (QmlSettingDialog::isPlaySound()) {
                QApplication::beep();
            }
        }

        int syncInterval = QmlSettingDialog::getSyncInterval();
        syncWaitTimer->start(syncInterval * 1000);
        loop.exec();
    }

    qDebug() << "mikan rss sync task quit!!!!!";
    deleteLater();
}
