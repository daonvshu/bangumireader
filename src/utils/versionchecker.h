#pragma once

#include <qobject.h>

class VersionChecker : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool newVersionDetect MEMBER newVersionDetect NOTIFY newVersionDetectChanged)
    Q_PROPERTY(QString newVersionString MEMBER newVersionString NOTIFY newVersionStringChanged)

public:
    VersionChecker();

signals:
    void newVersionDetectChanged();
    void newVersionStringChanged();

private:
    bool newVersionDetect;
    QString newVersionString;

private:
    void checkVersion();
    void parseVersionData(const QByteArray& data);
};