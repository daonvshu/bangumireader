#include "bangumidatamodel.h"

#include "dao.h"

BangumiDatabaseList BangumiDataModel::getLocalBangumiDataIdAll() {
    BangumiDatabase::Fields bf;
    return dao::_select<BangumiDatabase>().column(bf.bangumiId).build().list();
}

BangumiDatabaseList BangumiDataModel::getLocalBangumiDataAll(const QString& keywords, const QString& type) {
    BangumiDatabase::Fields bf;
    auto select = dao::_select<BangumiDatabase>().with(_orderBy(bf.beginDate.desc()));
    if (!keywords.isEmpty()) {
        select.filter(bf.title.like("%" + keywords + "%"));
    }
    if (!type.isEmpty()) {
        select.filter(bf.type == type);
    }
    return select.build().list();
}

BangumiDatabaseList BangumiDataModel::getStarStatusSince(qint64 timestamp) {
    BangumiDatabase::Fields bf;
    return dao::_select<BangumiDatabase>().column(bf.bangumiId, bf.star).filter(bf.beginDate >= timestamp).build().list();
}

void BangumiDataModel::clearDataSince(qint64 timestamp) {
    BangumiDatabase::Fields bf;
    dao::_delete<BangumiDatabase>().filter(bf.beginDate >= timestamp).build().deleteBy();
}

void BangumiDataModel::addData(const BangumiDatabaseList& data) {
    if (data.isEmpty()) {
        return;
    }
    dao::_insert<BangumiDatabase>().build().insert(data);
}

BangumiDatabase BangumiDataModel::getBangumi(int bangumiId) {
    BangumiDatabase::Fields bf;
    return dao::_select<BangumiDatabase>().filter(bf.bangumiId == bangumiId).build().one();
}

bool BangumiDataModel::changeStar(int bangumiId, int star) {
    BangumiDatabase::Fields bf;
    return dao::_update<BangumiDatabase>().set(bf.star = star).filter(bf.bangumiId == bangumiId).build().update();
}
