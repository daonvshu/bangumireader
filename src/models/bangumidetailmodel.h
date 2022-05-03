#pragma once

#include <qobject.h>

#include "entity/BangumiDatabase.h"

struct BangumiDetail {
    Q_GADGET

    Q_PROPERTY(QString airDate MEMBER airDate)
    Q_PROPERTY(int airWeekday MEMBER airWeekday)
    Q_PROPERTY(int eps MEMBER eps)
    Q_PROPERTY(QString image MEMBER image)
    Q_PROPERTY(QString summary MEMBER summary)

public:
    QString airDate; //放送日期
    int airWeekday = 0; //放送星期
    int eps = 0; //集数
    QString image;//缩略图
    QString summary;//简介
};
Q_DECLARE_METATYPE(BangumiDetail)

class BangumiDetailModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(int bangumiId READ getBangumiId WRITE setBangumiId)
    Q_PROPERTY(bool star READ getStar WRITE setStar NOTIFY starChanged)

    Q_PROPERTY(QVariant detail READ getDetail NOTIFY detailChanged)
    Q_PROPERTY(QVariant database READ getDatabase NOTIFY databaseChanged)

public:
    explicit BangumiDetailModel(QObject* parent = nullptr);

    int getBangumiId() const { return bangumiId; }
    void setBangumiId(int id);

    bool getStar() const { return star; }
    void setStar(bool star);

    QVariant getDetail() const { return QVariant::fromValue(bangumiDetail); }
    QVariant getDatabase() const { return QVariant::fromValue(bangumiDatabase); }

signals:
    void detailChanged();
    void databaseChanged();
    void starChanged();

private:
    int bangumiId = -1;
    bool star = false;

    BangumiDetail bangumiDetail;
    BangumiDatabase bangumiDatabase;

private:
    void getBangumiDetail();
    void parseDetailData(const QByteArray& data);
};