#include "bangumilistmodel.h"

#include "HttpClient.h"

#include <qxmlstream.h>
#include <qdebug.h>

BangumiListModel::BangumiListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int BangumiListModel::rowCount(const QModelIndex& parent) const {
    return formattedData.size();
}

QVariant BangumiListModel::data(const QModelIndex& index, int role) const {
    if (role == RoleType) {
        return formattedData[index.row()].canConvert<QList<BangumiData>>() ? 1 : 0;
    }
    if (role == RoleHeader) {
        return formattedData[index.row()].toInt();
    }
    return formattedData[index.row()];
}

QHash<int, QByteArray> BangumiListModel::roleNames() const {
    return {
        { RoleHeader, "header" },
        { RoleData, "contentData" },
        { RoleType, "type" },
    };
}

#define HOST_URL            "https://mikanani.me"
#define BANGUMI_REQUEST_URL QString(HOST_URL ## "/Home/BangumiCoverFlowByDayOfWeek?year=%1&seasonStr=%2")

void BangumiListModel::refreshData() {
    if (selectedYear.isEmpty() || selectedSeason.isEmpty()) {
        return;
    }

    static AeaQt::HttpClient client;
    //client.setNetworkAccessible(QNetworkAccessManager::Accessible);
    client.get(BANGUMI_REQUEST_URL.arg(selectedYear).arg(selectedSeason))
        .onFailed([&](const QByteArray& reason) {
        qDebug() << reason;
    })
        .onSuccess([&](const QByteArray& data) {
        parseHtmlData(data);
    })
        .onFinished([&] {

    })
        .exec();
}

void BangumiListModel::parseHtmlData(const QByteArray& data) {
    QXmlStreamReader reader("<div>" + data + "</div>");

    beginResetModel();
    bangumiData.clear();
    formattedData.clear();

    BangumiData currentBangumi;
    bool itemBegin = false;
    int currentWeek = -1;

    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == "div") {
                auto weekStr = reader.attributes().value("data-dayofweek");
                if (!weekStr.isEmpty()) {
                    currentWeek = weekStr.toInt();
                    bangumiData.insert(currentWeek, {});
                }
            }
            if (reader.name() == "li") {
                itemBegin = true;
                currentBangumi = BangumiData();
            }
            if (itemBegin) {
                if (reader.name() == "span") {
                    currentBangumi.thumbnail = reader.attributes().value("data-src").toString();
                } else if (reader.name() == "a") {
                    currentBangumi.link = reader.attributes().value("href").toString();
                    currentBangumi.title = reader.attributes().value("title").toString();
                } else if (reader.name() == "div" && currentBangumi.title.isEmpty()) {
                    auto title = reader.attributes().value("title");
                    if (!title.isEmpty()) {
                        currentBangumi.title = reader.attributes().value("title").toString();
                    }
                }
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (reader.name() == "li") {
                itemBegin = false;
                bangumiData[currentWeek] << currentBangumi;
            }
        }
    }
    if (reader.hasError()) {
        qDebug() << "reader error occur:" << reader.error() << reader.errorString();
    }
    qDebug() << "decode bangumi data size:" << bangumiData.size();
    /*
    for (const auto& w : bangumiData.keys()) {
        qDebug() << "week:" << w;
        for (const auto& d : bangumiData[w]) {
            qDebug() << d.toString();
        }
    }*/

    if (bangumiData.isEmpty()) {
        endResetModel();
        return;
    }
    //format data
    for (int i = 0; i <= 8; i++) {
        const auto& d = bangumiData[i];
        if (d.isEmpty()) {
            continue;
        }
        formattedData.append(i);

#define COL_COUNT   5
        int rowSize = (d.size() - 1) / COL_COUNT + 1;
        for (int p = 0; p < rowSize; p++) {
            formattedData.append(QVariant::fromValue(d.mid(p * COL_COUNT, COL_COUNT)));
        }
    }
    endResetModel();
}