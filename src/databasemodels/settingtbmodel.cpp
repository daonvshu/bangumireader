#include "settingtbmodel.h"

#include "dao.h"
#include "entity/SettingTb.h"

#include <qstandardpaths.h>

QString SettingTbModel::getLastSaveDirectory() {
    SettingTb::Fields sf;
    auto d = dao::_select<SettingTb>()
        .filter(sf.name == "lastSavePath")
        .build().unique();
    if (d.getData().toString().isEmpty()) {
        return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
    return d.getData().toString();
}

void SettingTbModel::updateSaveDirectory(const QString& path) {
    SettingTb setting("lastSavePath", path);
    dao::_insert<SettingTb>().build().insertOrReplace(setting);
}
