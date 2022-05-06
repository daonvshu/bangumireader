#include "bugreport.h"

#include <qfileinfo.h>
#include <qdatetime.h>
#include <qmath.h>
#include <qprocess.h>
#include <qstringlistmodel.h>
#include <qdesktopservices.h>
#include <qurl.h>

struct CrashInfo {
    QString crashSavePath;
    QString miniDumpId;
    QString accessKey;
    QString appName;

    CrashInfo() {
        auto args = qApp->arguments();
        args.takeFirst();

        crashSavePath = args.takeFirst();
        miniDumpId = args.takeFirst();
        accessKey = args.takeFirst();
        appName = args.takeFirst();
    }

    qint64 fileSaveTimestamp() const {
        QFileInfo info(crashSavePath + "/" + miniDumpId + ".dmp");
        return info.lastModified().toSecsSinceEpoch();
    }

    QString crashFilePath() const {
        return crashSavePath + "/" + miniDumpId + ".dmp";
    }

    QString logFilePath() const {
        return crashSavePath + "/" + miniDumpId + ".log";
    }

    void clearFile() const {
        QFile::remove(crashFilePath());
        QFile::remove(logFilePath());
    }
};

BugReport::BugReport(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    crashInfo = new CrashInfo;

    auto model = new QStringListModel(ui.file_list);
    model->setStringList(QStringList() << crashInfo->miniDumpId + ".dmp" << crashInfo->miniDumpId + ".log");
    ui.file_list->setModel(model);
}

BugReport::~BugReport() {
    delete crashInfo;
}

void BugReport::on_btn_close_clicked() {
    if (ui.checkboxEx_restart->isChecked()) {
        QProcess::startDetached(crashInfo->appName);
    }
    close();
}

void BugReport::on_btn_open_clicked() {
    auto path = crashInfo->crashFilePath();
    path.replace("/", "\\");
    QProcess::startDetached("explorer /select," + path);

    QDesktopServices::openUrl(QUrl("https://github.com/daonvshu/bugreportclient/issues"));
}
