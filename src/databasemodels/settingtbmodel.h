#pragma once

#include <qobject.h>

class SettingTbModel {
public:
    static QString getLastSaveDirectory();
    static void updateSaveDirectory(const QString& path);
};

class QmlSettingDialog: public QObject {
    Q_OBJECT

    Q_PROPERTY(bool playSound READ isPlaySound WRITE setPlaySound)
    Q_PROPERTY(int syncInterval READ getSyncInterval WRITE setSyncInterval)
    Q_PROPERTY(bool autoStart READ isAutoStart WRITE setAutoStart)

public:
    static bool isPlaySound();
    static void setPlaySound(bool play);

    static int getSyncInterval();
    static void setSyncInterval(int interval);

    static bool isAutoStart();
    static void writeAutoStartDefault();
    static void setAutoStart(bool autoStart);
};