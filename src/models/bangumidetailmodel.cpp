#include "bangumidetailmodel.h"

#include "databasemodels/bangumidatamodel.h"

#include "HttpClient.h"

#include <qdebug.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

BangumiDetailModel::BangumiDetailModel(QObject* parent)
    : QObject(parent)
{
    
}

void BangumiDetailModel::setBangumiId(int id) {
    bangumiId = id;
    bangumiDatabase = BangumiDataModel::getBangumi(bangumiId);
    emit databaseChanged();

    star = bangumiDatabase.getStar() == 1;
    emit starChanged();

    getBangumiDetail();
}

void BangumiDetailModel::setStar(bool star) {
    int starValue = star ? 1 : 0;
    if (BangumiDataModel::changeStar(bangumiId, starValue)) {
        bangumiDatabase.setStar(starValue);
        this->star = star;
        emit starChanged();
    }
}

#define URL_TAG "https://api.bgm.tv/subject/%1?responseGroup=small"

void BangumiDetailModel::getBangumiDetail() {
    static AeaQt::HttpClient client;
    client.get(QString(URL_TAG).arg(bangumiId))
        .onFailed([=](const QByteArray& reason) {
            qDebug() << reason;
        })
        .onSuccess([=](const QByteArray& data) {
            parseDetailData(data);
        })
    .exec();
}

void BangumiDetailModel::parseDetailData(const QByteArray& data) {
    auto document = QJsonDocument::fromJson(data);
    if (document.isNull()) {
        qDebug() << "json parse failed:" << data;
        return;
    }

    auto obj = document.object();
    bangumiDetail.airDate = obj.value("air_date").toString();
    bangumiDetail.airWeekday = obj.value("air_weekday").toInt();
    bangumiDetail.eps = obj.value("eps").toInt();
    bangumiDetail.summary = obj.value("summary").toString();

    auto imagesObj = obj.value("images").toObject();
    bangumiDetail.image = imagesObj.value("large").toString();

    emit detailChanged();
}
