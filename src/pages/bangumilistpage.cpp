#include "bangumilistpage.h"

#include "HttpClient.h"
#include "SmoothScrollbar.h"

#include <qxmlstream.h>
#include <qdebug.h>

int BangumiListPage::typeId = qRegisterMetaType<BangumiListPage*>();
BangumiListPage::BangumiListPage(const QVariant& data, QWidget* parent)
    : AbstractRouterWidget(data, parent)
{
    ui.setupUi(this);
    ui.scrollArea->setVerticalScrollBar(new SmoothScrollbar(this));
}

BangumiListPage::~BangumiListPage()
{
}

void BangumiListPage::runRouterEvent(const QString& event, const QVariant& data) {
    if (event == "reload") {
        auto d = data.value<QPair<QString, QString>>();
        requestBangumiData(d.first, d.second);
    }
}

#define HOST_URL            "https://mikanani.me"
#define BANGUMI_REQUEST_URL QString(HOST_URL ## "/Home/BangumiCoverFlowByDayOfWeek?year=%1&seasonStr=%2")

void BangumiListPage::requestBangumiData(const QString& year, const QString& season) {
    static AeaQt::HttpClient client;
    client.setNetworkAccessible(QNetworkAccessManager::Accessible);
    client.get(BANGUMI_REQUEST_URL.arg(year).arg(season))
        .onFailed([&] (const QByteArray& reason) {
            qDebug() << reason;
        })
        .onSuccess([&](const QByteArray& data) {
            parseHtmlData(data);
        })
        .onFinished([&] {

        })
    .exec();
}

void BangumiListPage::parseHtmlData(const QByteArray& data) {
    QXmlStreamReader reader("<div>" + data + "</div>");

    QMap<int, QList<BangumiData>> bangumiData;

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
    for (const auto& w : bangumiData.keys()) {
        qDebug() << "week:" << w;
        for (const auto& d : bangumiData[w]) {
            qDebug() << d.toString();
        }
    }
    showBangumiList(bangumiData);
}

void BangumiListPage::showBangumiList(const QMap<int, QList<BangumiData>>& data) {
    //clear previous data
    QLayoutItem* item;
    while ((item = ui.listcontent->layout()->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }


}
