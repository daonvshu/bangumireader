#include "versionchecker.h"

#include "HttpClient.h"

#include "databasemodels/settingtbmodel.h"

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

VersionChecker::VersionChecker()
    : newVersionDetect(false)
{
    checkVersion();
}

#define URL_TAG "https://api.github.com/repos/daonvshu/bangumireader/tags"
void VersionChecker::checkVersion() {
    static AeaQt::HttpClient client;
    client.get(URL_TAG)
        .onFailed([=](const QByteArray& reason) {
            qDebug() << reason;
        })
        .onSuccess([&](const QByteArray& data) {
        parseVersionData(data);
    }).exec();
}

void VersionChecker::parseVersionData(const QByteArray& data) {
    auto document = QJsonDocument::fromJson(data);
    if (document.isNull()) {
        return;
    }
    if (!document.isArray()) {
        return;
    }
    auto versionArr = document.array();
    if (versionArr.isEmpty()) {
        return;
    }
    auto lastVersion = versionArr[0].toObject().value("name").toString();
    if (lastVersion != SettingTbModel::getLastCheckVersion()) {
        newVersionDetect = true;
        emit newVersionDetectChanged();

        newVersionString = lastVersion;
        emit newVersionStringChanged();

        SettingTbModel::saveLastCheckVersion(lastVersion);
    }
}
