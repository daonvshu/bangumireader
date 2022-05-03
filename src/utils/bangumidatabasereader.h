#pragma once

#include <qobject.h>
#include <qthread.h>
#include <qmutex.h>

class BangumiDatabaseReader : public QThread {
    Q_OBJECT

public:
    explicit BangumiDatabaseReader(QObject* parent = nullptr);

signals:
    void downloadFailed(const QString& reason);
    void downloadProgress(const QString& percentage);
    void downloadStep(const QString& step);
    void downloadFinished();

protected:
    void run() override;

private:
    void databaseUpdate();
    void parseDatabaseSource(const QByteArray& data);

private:
    QMutex progressUpdateLock;
    QString percentageData;
};