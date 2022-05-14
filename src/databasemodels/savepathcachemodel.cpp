#include "savepathcachemodel.h"

#include <qdir.h>

#include "dao.h"

#include "entity/SavePathCache.h"

#include "settingtbmodel.h"

QString SavePathCacheModel::getLastSaveDirectory(int bangumiId) {

    SavePathCache::Fields sf;
    auto d = dao::_select<SavePathCache>()
        .column(sf.savePath)
        .filter(sf.bangumiId == bangumiId)
        .build().unique();
    if (d.getSavePath().isEmpty()) {

        //use last one
        auto l = dao::_select<SavePathCache>()
            .column(sf.savePath)
            .with(_orderBy(sf.createTime.desc()), _limit(1))
            .build().one();
        if (!l.getSavePath().isEmpty()) {
            QDir dir(l.getSavePath());
            dir.cdUp();
            return dir.absolutePath();
        }

        return SettingTbModel::getLastSaveDirectory();
    }
    return d.getSavePath();
}

void SavePathCacheModel::updateSaveDirectory(int bangumiId, const QString& path) {
    SavePathCache setting(bangumiId, path, QDateTime::currentSecsSinceEpoch());
    dao::_insert<SavePathCache>().build().insertOrReplace(setting);
}