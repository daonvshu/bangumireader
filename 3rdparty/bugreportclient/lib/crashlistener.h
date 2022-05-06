#pragma once

#include <qobject.h>

namespace google_breakpad {
    class ExceptionHandler;
}

class CrashListener {
    Q_DISABLE_COPY(CrashListener)

public:
    explicit CrashListener(const QString& accessKey);
    ~CrashListener();

    static QString getCacheDir();

    const QStringList& getCache() const {
        return cacheLogs;
    }

    const QString& getAccessKey() const {
        return accessKey;
    }

private:
    google_breakpad::ExceptionHandler* eh;
    QStringList cacheLogs;
    QString accessKey;
};