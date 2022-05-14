#pragma once

#include <qobject.h>

class SavePathCacheModel {
public:
    static QString getLastSaveDirectory(int bangumiId);
    static void updateSaveDirectory(int bangumiId, const QString& path);
};