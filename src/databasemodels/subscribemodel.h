#pragma once

#include <qobject.h>

#include "entity/SubscribeTb.h"
#include "entity/SubscribeGroups.h"
#include "entity/SubscribeGroupItem.h"

class SubScribeGroupsModel {
public:
    static SubscribeGroups getGroup(int bangumiId, const QString& groupName);
    static void updateSubscribeGroup(SubscribeGroups& subscribeGroup);
    static void removeSubscribeGroup(const SubscribeGroups& subscribeGroup);

    static void insertSubscribeGroupItems(int groupId, const QStringList& links);
    static void removeSubscribeGroupItems(int groupId);
    static SubscribeGroupItemList getAllSubscribeGroupItems(int groupId);
    static SubscribeGroupItemList getAllSubscribeGroupItems(int bangumiId, const QString& groupName);
    static void removeTargetSubscribeGroupItems(const QList<int>& itemIds);
    static bool isSubscribeGroupItemNew(const QString& link);
    static void removeSubscribeGroupItemNewStatus(const QString& link);

    static SubscribeTbList getAllSubscribeTarget();
    static SubscribeTbList getAllSubscribeInfo();
    static QString getSubscribeTargetTitle(int bangumiId);
};