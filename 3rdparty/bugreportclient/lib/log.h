#pragma once

#include <qhostinfo.h>
#include <qtcpsocket.h>

#include <qqueue.h>
#include <qmutex.h>

#include <qthread.h>

enum LogLevel {
    LEVEL_DEBUG = 0,
    LEVEL_WARNING,
    LEVEL_ERROR,
};

struct LogData;
class Log : public QThread {
    Q_OBJECT

    Log();
    ~Log();

    Q_DISABLE_COPY(Log);

    static void createInstance();

public:
    static void waitForConnect(const QHostAddress& address, int port);
    static void useQDebugOnly();
    static void saveInFile(const QString& path);
    static void setLogDataHandler(std::function<void(const QString&)> logDataHandler);

    static void setCurrentThreadName(const QString& name);

    static void threadExit();

    class Message {
        Q_DISABLE_COPY(Message);

    public:
        Message(const QString& tag, LogLevel level, const char* fileName, int line);

        Message(LogLevel level, const char* fileName, int line);

        Message(const QString& tag, LogLevel level);

        ~Message();

        QDebug logger;

    private:
        QString tag, fileName;
        int line;
        LogLevel level;
        bool tagIsFile;

        QString cacheString;
    };

signals:
    void newLogArrived(QPrivateSignal);
    void logReadyPost(QPrivateSignal);

private:
    QHash<int64_t, QString> threadNames;
    QQueue<LogData> logQueue;
    QMutex logQueueLock;
    bool onlyQDebugPrint;

    QHostAddress address;
    int port;
    QMutex nameSetLock;

    QString saveTargetFilePath;

    std::function<void(const QString&)> logDataHandler;

    static Log* m_log;

protected:
    void run() override;

private:
    static void addLog(LogData& data);

    QByteArray getProcessInfo();
};

#define Log_D(tag)     Log::Message(tag, LEVEL_DEBUG, __FILE__, __LINE__).logger
#define Log_d             Log::Message(LEVEL_DEBUG, __FILE__, __LINE__).logger

#define Log_W(tag)    Log::Message(tag, LEVEL_WARNING, __FILE__, __LINE__).logger
#define Log_w             Log::Message(LEVEL_WARNING, __FILE__, __LINE__).logger

#define Log_E(tag)     Log::Message(tag, LEVEL_ERROR, __FILE__, __LINE__).logger
#define Log_e             Log::Message(LEVEL_ERROR, __FILE__, __LINE__).logger