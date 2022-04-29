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
