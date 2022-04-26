#pragma once

#include <qobject.h>

class RemarkTbModel {
public:
    static bool checkLinkExist(const QString& link);
    static void cacheNewDownloadedLink(const QString& link);
};