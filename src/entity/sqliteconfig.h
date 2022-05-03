#pragma once

#include <qobject.h>

class SqliteConfig : public QObject {
    Q_OBJECT

    Q_PROPERTY(int version READ getVersion())
    Q_PROPERTY(QString type READ getSqlType())
    Q_PROPERTY(QString dbName READ getDatabaseName())

public:
    int getVersion() {
        return 3;
    }

    QString getSqlType() {
        return "sqlite";
    }

    QString getDatabaseName() {
        return "mikan_bangumi_reader";
    }
};