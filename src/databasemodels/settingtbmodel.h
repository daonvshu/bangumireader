#pragma once

#include <qobject.h>

class SettingTbModel {
public:
    static QString getLastSaveDirectory();
    static void updateSaveDirectory(const QString& path);
};
