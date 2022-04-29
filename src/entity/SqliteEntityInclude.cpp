#include "SqliteEntityInclude.h"
#include "SettingTb.h"
#include "RemarkTb.h"
#include "SubscribeTb.h"
#include "SubscribeGroups.h"
#include "SubscribeGroupItem.h"

#include "DbLoader.h"
namespace DaoSqlite {
    void SqliteEntityDelegate::createEntityTables() {
        DbLoader::getClient().createTables<ClientSqlite, SettingTb, RemarkTb, SubscribeTb, SubscribeGroups, SubscribeGroupItem>();
    }

    void SqliteEntityDelegate::entityTablesUpgrade() {
        DbLoader::getClient().tablesUpgrade<ClientSqlite, SettingTb, RemarkTb, SubscribeTb, SubscribeGroups, SubscribeGroupItem>();
    }

    const int entitySqliteDelegateId = qRegisterMetaType<SqliteEntityDelegate*>();
}

