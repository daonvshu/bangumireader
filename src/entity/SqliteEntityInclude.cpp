#include "SqliteEntityInclude.h"
#include "SettingTb.h"
#include "RemarkTb.h"
#include "SubscribeTb.h"
#include "SubscribeGroups.h"
#include "SubscribeGroupItem.h"
#include "BangumiDatabase.h"
#include "SavePathCache.h"

#include "DbLoader.h"
namespace DaoSqlite {
    void SqliteEntityDelegate::createEntityTables() {
        DbLoader::getClient().createTables<ClientSqlite, SettingTb, RemarkTb, SubscribeTb, SubscribeGroups, SubscribeGroupItem, BangumiDatabase, SavePathCache>();
    }

    void SqliteEntityDelegate::entityTablesUpgrade() {
        DbLoader::getClient().tablesUpgrade<ClientSqlite, SettingTb, RemarkTb, SubscribeTb, SubscribeGroups, SubscribeGroupItem, BangumiDatabase, SavePathCache>();
    }

    const int entitySqliteDelegateId = qRegisterMetaType<SqliteEntityDelegate*>();
}

