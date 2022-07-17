#include "subscribemodel.h"

#include "dao.h"

SubscribeGroups SubScribeGroupsModel::getGroup(int bangumiId, const QString& groupName) {
    SubscribeGroups::Fields sf;
    return dao::_select<SubscribeGroups>().filter(sf.bangumiId == bangumiId, sf.groupName == groupName).build().unique();
}

void SubScribeGroupsModel::updateSubscribeGroup(SubscribeGroups& subscribeGroup) {
    if (subscribeGroup.getId() == -1) {
        dao::_insert<SubscribeGroups>().build().insert(subscribeGroup);

        SubscribeTb::Fields stf;
        if (dao::_count<SubscribeTb>().filter(stf.bangumiId == subscribeGroup.getBangumiId()).count() == 0) {
            SubscribeTb subscribe(subscribeGroup.getBangumiId(), 
                subscribeGroup.__getExtra("title").toString(), 
                subscribeGroup.__getExtra("rssLink").toString()
            );
            dao::_insert<SubscribeTb>().build().insert(subscribe);
        }
        return;
    }
    SubscribeGroups::Fields sf;
    dao::_update<SubscribeGroups>()
        .set(sf.keywords = subscribeGroup.getKeywords())
        .filter(sf.id == subscribeGroup.getId()).build()
        .update();
}

void SubScribeGroupsModel::removeSubscribeGroup(const SubscribeGroups& subscribeGroup) {
    dao::_delete<SubscribeGroups>().build().deleteBy(subscribeGroup);

    SubscribeGroups::Fields sf;
    if (dao::_count<SubscribeGroups>().filter(sf.bangumiId == subscribeGroup.getBangumiId()).count() == 0) {
        SubscribeTb::Fields stf;
        dao::_delete<SubscribeTb>().filter(stf.bangumiId == subscribeGroup.getBangumiId()).build().deleteBy();
    }
}

QStringList SubScribeGroupsModel::getSubscribedGroups(int bangumiId) {
    SubscribeGroups::Fields sf;
    auto data = dao::_select<SubscribeGroups>().column(sf.groupName).filter(sf.bangumiId == bangumiId).build().list();
    QStringList groups;
    for (const auto& d : data) {
        groups << d.getGroupName();
    }
    return groups;
}

void SubScribeGroupsModel::insertSubscribeGroupItems(int groupId, const QStringList& links) {
    SubscribeGroupItem::Fields sif;
    QList<int> groups;
    for (int i = 0; i < links.size(); i++) {
        groups.append(groupId);
    }
    dao::_insert<SubscribeGroupItem>().set(sif.groupId = groups, sif.sourceLink = links).build().insert();
}

void SubScribeGroupsModel::removeSubscribeGroupItems(int groupId) {
    SubscribeGroupItem::Fields sif;
    dao::_delete<SubscribeGroupItem>().filter(sif.groupId = groupId).build().deleteBy();
}

SubscribeGroupItemList SubScribeGroupsModel::getAllSubscribeGroupItems(int groupId) {
    SubscribeGroupItem::Fields sif;
    return dao::_select<SubscribeGroupItem>().filter(sif.groupId = groupId).build().list();
}

SubscribeGroupItemList SubScribeGroupsModel::getAllSubscribeGroupItems(int bangumiId, const QString& groupName) {
    SubscribeGroups::Fields sf;
    SubscribeGroupItem::Fields sif;

    auto results = dao::_join<SubscribeGroups, SubscribeGroupItem>()
        .columnAll<SubscribeGroupItem>()
        .from<SubscribeGroups>()
        .innerJoin<SubscribeGroupItem>().on(sif.groupId == sf.id)
        .filter(sf.bangumiId == bangumiId, sf.groupName == groupName)
        .build().list();
    SubscribeGroupItemList data;
    for (const auto& res: results) {
        data << std::get<1>(res);
    }
    return data;
}

void SubScribeGroupsModel::removeTargetSubscribeGroupItems(const QList<int>& itemIds) {
    SubscribeGroupItem::Fields sif;
    dao::_delete<SubscribeGroupItem>().filter(sif.id == itemIds).build().deleteBatch();
}

bool SubScribeGroupsModel::isSubscribeGroupItemNew(const QString& link) {
    SubscribeGroupItem::Fields sif;
    auto d = dao::_select<SubscribeGroupItem>()
        .filter(/*sif.groupId == groupId, */sif.sourceLink == link)
        .build().unique();
    return d.getId() != -1 && d.getNewItem() == 1;
}

void SubScribeGroupsModel::removeSubscribeGroupItemNewStatus(const QString& link) {
    SubscribeGroupItem::Fields sif;
    dao::_update<SubscribeGroupItem>().set(sif.newItem = 0).filter(sif.sourceLink == link).build().update();
}

SubscribeTbList SubScribeGroupsModel::getAllSubscribeTarget() {
    return dao::_selectAll<SubscribeTb>();
}

SubscribeTbList SubScribeGroupsModel::getAllSubscribeInfo() {
    SubscribeTb::Fields stf;
    SubscribeGroups::Fields sgf;
    SubscribeGroupItem::Fields sif;

    auto d = dao::_join<SubscribeTb, SubscribeGroups, SubscribeGroupItem>()
        .columnAll<SubscribeTb>()
        .column(_fun("sum(%1) as newSize").field(sif.newItem))
        .from<SubscribeTb>()
        .innerJoin<SubscribeGroups>().on(sgf.bangumiId == stf.bangumiId)
        .innerJoin<SubscribeGroupItem>().on(sif.groupId == sgf.id)
        .with(_groupBy(stf.bangumiId))
        .build().list();

    SubscribeTbList results;
    for (const auto& res: d) {
        SubscribeTb stb = std::get<0>(res);
        SubscribeGroupItem sgi = std::get<2>(res);
        stb.__putExtra("newSize", sgi.__getExtra("newSize"));
        results << stb;
    }
    return results;
}

QString SubScribeGroupsModel::getSubscribeTargetTitle(int bangumiId) {
    SubscribeTb::Fields stf;
    return dao::_select<SubscribeTb>().filter(stf.bangumiId == bangumiId).build().one().getTitle();
}
