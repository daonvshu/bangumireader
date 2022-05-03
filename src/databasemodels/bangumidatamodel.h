#pragma once

#include <qobject.h>

#include "entity/BangumiDatabase.h"

class BangumiDataModel {
public:
    static BangumiDatabaseList getLocalBangumiDataIdAll();
    static BangumiDatabaseList getLocalBangumiDataAll(const QString& keywords, const QString& type);
    static BangumiDatabaseList getStarStatusSince(qint64 timestamp);
    static void clearDataSince(qint64 timestamp);
    static void addData(const BangumiDatabaseList& data);
};