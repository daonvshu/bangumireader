#include "settingtbmodel.h"

#include <QApplication>

#include "dao.h"
#include "entity/SettingTb.h"

#include <qstandardpaths.h>
#include <qsettings.h>

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

QString SettingTbModel::getLastCheckVersion() {
    SettingTb::Fields sf;
    auto d = dao::_select<SettingTb>()
        .filter(sf.name == "lastCheckVersion")
        .build().unique();
    if (d.getData().toString().isEmpty()) {
        return APP_VERSION;
    }
    return d.getData().toString();
}

void SettingTbModel::saveLastCheckVersion(const QString& versionStr) {
    SettingTb setting("lastCheckVersion", versionStr);
    dao::_insert<SettingTb>().build().insertOrReplace(setting);
}

bool QmlSettingDialog::isPlaySound() {
    SettingTb::Fields sf;
    auto d = dao::_select<SettingTb>()
        .filter(sf.name == "playSound")
        .build().unique();
    return d.getData().toBool();
}

void QmlSettingDialog::setPlaySound(bool play) {
    SettingTb setting("playSound", play);
    dao::_insert<SettingTb>().build().insertOrReplace(setting);
}

int QmlSettingDialog::getSyncInterval() {
    SettingTb::Fields sf;
    auto d = dao::_select<SettingTb>()
        .filter(sf.name == "syncInterval")
        .build().unique();
    if (d.getName().isEmpty()) {
        return 1 * 3600;
    }
    return d.getData().toInt();
}

void QmlSettingDialog::setSyncInterval(int interval) {
    SettingTb setting("syncInterval", interval);
    dao::_insert<SettingTb>().build().insertOrReplace(setting);
}

void applyRegister(const std::function<void(QSettings&, const QString&, const QString&)>& apply) {
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    auto appName = QApplication::applicationName();
    auto appPath = QApplication::applicationFilePath().replace("/", "\\").prepend("\"").append("\"");
    apply(settings, appName, appPath);
}

bool QmlSettingDialog::isAutoStart() {
    //check setting auto start
    SettingTb::Fields sf;
    auto d = dao::_select<SettingTb>()
        .filter(sf.name == "autoStart")
        .build().unique();
    bool currentIsAutoStart = d.getData().toBool(); //default is false

    //check register is auto start
    if (currentIsAutoStart) {
        applyRegister([](QSettings& settings, const QString& appName, const QString& appPath) {
            if (settings.value(appName).toString() != appPath) {
                settings.setValue(appName, appPath);
            }
        });
    }

    return currentIsAutoStart;
}

void QmlSettingDialog::writeAutoStartDefault() {
    SettingTb::Fields sf;
    auto d = dao::_select<SettingTb>()
        .filter(sf.name == "autoStart")
        .build().unique();
    if (d.getName().isEmpty()) { //no such config
        setAutoStart(true);
    } else {
        if (d.getData().toBool()) {
            //check app directory changed
            applyRegister([](QSettings& settings, const QString& appName, const QString& appPath) {
                if (settings.value(appName).toString() != appPath) {
                    settings.setValue(appName, appPath);
                }
            });
        }
    }
}

void QmlSettingDialog::setAutoStart(bool autoStart) {
    SettingTb setting("autoStart", autoStart);
    dao::_insert<SettingTb>().build().insertOrReplace(setting);

    applyRegister([&](QSettings& settings, const QString& appName, const QString& appPath) {
        if (autoStart) {
            settings.setValue(appName, appPath);
        } else {
            settings.remove(appName);
        }
    });
}
