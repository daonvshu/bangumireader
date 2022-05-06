#include "crashlistener.h"

#include <qdir.h>
#include <qstandardpaths.h>
#include <qprocess.h>
#include <qcoreapplication.h>

#include "log.h"
#include "handler/exception_handler.h"

#ifdef __GNUC__
bool dumpResultsHandler(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded) {
#else
bool dumpResultsHandler(const wchar_t*, const wchar_t* minidump_id, void* context, EXCEPTION_POINTERS*, MDRawAssertionInfo*, bool succeeded) {
#endif
    auto listener = static_cast<CrashListener*>(context);

#ifdef __GNUC__
    QFileInfo fileInfo(descriptor.path());
    auto minidumpFileName = fileInfo.baseName();
#else
    auto minidumpFileName = QString::fromStdWString(minidump_id);
#endif

    if (!listener->getCache().isEmpty()) {
        QFile file(CrashListener::getCacheDir() + "/" + minidumpFileName + ".log");
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            for (const auto& it : listener->getCache()) {
                file.write(it.toUtf8() + '\n');
            }
            file.close();
        }
    }
    QStringList args;
    args << CrashListener::getCacheDir();
    args << minidumpFileName;
    args << listener->getAccessKey();
    args << QCoreApplication::applicationName();
    QProcess::startDetached(BUGREPORT_EXECUTABLE, args);

    return succeeded;
}

void crashListenerMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    static QMutex mutex;
    mutex.lock();

    QString log = msg;
    QString tag = "";
    if (context.file) {
        QFileInfo info(context.file);
        tag = info.baseName();
        log = QString("%1 (line:%3)").arg(msg).arg(context.line);
    }
    switch (type) {
        case QtDebugMsg:
        case QtInfoMsg:
            Log_D(tag) << log;
            break;
        case QtWarningMsg:
            Log_W(tag) << log;
            break;
        case QtCriticalMsg:
        case QtFatalMsg:
            Log_E(tag) << log;
            break;
    }
    mutex.unlock();
}

CrashListener::CrashListener(const QString& accessKey)
    : accessKey(accessKey)
{
    qInstallMessageHandler(crashListenerMessageOutput);
#ifdef __GNUC__
    eh = new google_breakpad::ExceptionHandler(google_breakpad::MinidumpDescriptor(getCacheDir().toStdString()), nullptr, dumpResultsHandler, this, true, -1);
#else
    eh = new google_breakpad::ExceptionHandler(getCacheDir().toStdWString(), nullptr, dumpResultsHandler, this, true, nullptr);
#endif
    Log::setLogDataHandler([&](const QString& log) {
        cacheLogs.append(log);
        if (cacheLogs.size() > 3000) {
            cacheLogs.removeFirst();
        }
    });
}

CrashListener::~CrashListener() {
    delete eh;
}

QString CrashListener::getCacheDir() {
    auto dir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir path(dir);
    if (!path.exists()) {
        path.mkpath(dir);
    }
    return dir;
}
