#include "bangumidatabasereader.h"

#define URL_TAG "https://cdn.jsdelivr.net/npm/bangumi-data@0.3/dist/data.json"

#include "HttpClient.h"
#include "dao.h"

#include <qdebug.h>
#include <qeventloop.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "databasemodels/bangumidatamodel.h"

BangumiDatabaseReader::BangumiDatabaseReader(QObject* parent)
    : QThread(parent)
{
}

void BangumiDatabaseReader::run() {
    databaseUpdate();

    ConnectionPool::closeConnection();
    deleteLater();
}

void BangumiDatabaseReader::databaseUpdate() {
    QEventLoop loop;

    emit downloadStep(QStringLiteral("正在下载数据..."));
    AeaQt::HttpClient client;
    client.get(URL_TAG)
        .onDownloadProgress([&](qint64 current, qint64 total) {
            emit downloadProgress(QStringLiteral("下载中：%1/%2").arg(current).arg(total));
        })
        .onFailed([&](const QByteArray& reason) {
            qDebug() << reason;
            emit downloadFailed(reason);
        })
        .onSuccess([&](const QByteArray& data) {
            parseDatabaseSource(data);
            emit downloadFinished();
        })
        .onFinished([&] {
            loop.quit();
        })
    .exec();

    loop.exec();
}

void BangumiDatabaseReader::parseDatabaseSource(const QByteArray& data) {
    emit downloadStep(QStringLiteral("正在处理数据中..."));

    auto document = QJsonDocument::fromJson(data);
    if (document.isNull()) {
        emit downloadFailed(QStringLiteral("远程资源格式错误！"));
        return;
    }

    auto rootObj = document.object();
    if (!rootObj.contains("siteMeta") || !rootObj.contains("items")) {
        emit downloadFailed(QStringLiteral("远程资源数据无效！"));
        return;
    }

    auto siteMetaObj = rootObj.value("siteMeta").toObject();
    auto itemsObj = rootObj.value("items").toArray();

    QList<BangumiDatabase> bangumiData;

    for (const auto& item: itemsObj) {
        auto itemObj = item.toObject();

        auto itemSites = itemObj.value("sites").toArray();
        int bangumiId = -1;
        QString sites;

        for (const auto& site: itemSites) {
            auto siteObj = site.toObject();
            auto idStr = siteObj.value("id").toString();
            if (!idStr.isEmpty()) {
                auto siteTag = siteObj.value("site").toString();
                if (siteTag == "bangumi") {
                    bangumiId = idStr.toInt();
                }
                if (siteMetaObj.contains(siteTag)) {
                    auto urlTemplate = siteMetaObj.value(siteTag).toObject().value("urlTemplate").toString();
                    if (!urlTemplate.isEmpty()) {
                        sites += siteTag + ',' + urlTemplate.replace("{{id}}", siteObj.value("id").toString()) + ',';
                    }
                }
            }
        }

        if (bangumiId != -1) {
            auto title = itemObj.value("title").toString();
            if (itemObj.contains("titleTranslate")) {
                auto transObj = itemObj.value("titleTranslate").toObject();
                if (transObj.contains("zh-Hans")) {
                    auto zhObj = transObj.value("zh-Hans").toArray();
                    if (!zhObj.isEmpty()) {
                        title = zhObj.last().toString();
                    }
                }
            }
            bangumiData.append(BangumiDatabase(
                bangumiId, title, 
                itemObj.value("type").toString(), 
                itemObj.value("officialSite").toString(),
                QDateTime::fromString(itemObj.value("begin").toString(), "yyyy-MM-ddTHH:mm:ss.zzzZ").toMSecsSinceEpoch(),
                sites, 0
            ));
        }
    }

    emit downloadStep(QStringLiteral("正在保存中..."));

    auto now = QDateTime::currentDateTime();
    now = now.addYears(-1);
    qint64 nowTimestamp = now.toMSecsSinceEpoch();

    auto sinceOneYearStarStatus = BangumiDataModel::getStarStatusSince(nowTimestamp);
    QHash<qint64, int> starStatus;
    for (const auto& item: sinceOneYearStarStatus) {
        starStatus.insert(item.getBangumiId(), item.getStar());
    }
    //remove data from one year old
    BangumiDataModel::clearDataSince(nowTimestamp);

    QSet<int> containsId;
    auto localStore = BangumiDataModel::getLocalBangumiDataIdAll();
    for (const auto& b: localStore) {
        containsId.insert(b.getBangumiId());
    }

    for (int i = bangumiData.size() - 1; i >= 0; i--) {
        if (containsId.contains(bangumiData[i].getBangumiId())) {
            bangumiData.removeAt(i);
            continue;
        }
        if (starStatus.contains(bangumiData[i].getBangumiId())) {
            bangumiData[i].setStar(starStatus.value(bangumiData[i].getBangumiId()));
        }
        containsId.insert(bangumiData[i].getBangumiId());
    }

    qDebug() << "receive new bangumi data size:" << bangumiData.size();
    BangumiDataModel::addData(bangumiData);
}
