#include "mikanrssreader.h"

#include "HttpClient.h"

#include <qxmlstream.h>
#include <qdebug.h>

MikanRssReader::MikanRssReader(QObject *parent)
    : QObject(parent)
{
}

#define BANGUMI_RSS_URL QString("https://mikanani.me/RSS/Bangumi?bangumiId=%1")

QString MikanRssReader::rssLink(int bangumiId) {
    return BANGUMI_RSS_URL.arg(bangumiId);
}

void MikanRssReader::readRssContent(int bangumiId, const std::function<void(const QMap<QString, QList<MikanTorrentLinkData>>&)>& groupDataHandler) {
    static AeaQt::HttpClient client;
    client.get(rssLink(bangumiId))
        .onFailed([&](const QByteArray& reason) {
            qDebug() << reason;
    })
        .onSuccess([=](const QByteArray& data) {
            parseRssContent(data, groupDataHandler);
    })
        .exec();
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

    for (const auto& d : rssData) {
        qDebug() << d.toString();
    }

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

    qDebug() << "grouped torrent link data:";
    for (const auto& key: groupLinkData.keys()) {
        qDebug() << "group:" << key;
        for (const auto& i : groupLinkData[key]) {
            qDebug() << i.toString();
        }
    }

    groupDataHandler(groupLinkData);
}

