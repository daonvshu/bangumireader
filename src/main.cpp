#include <qapplication.h>
#include <qqmlapplicationengine.h>
#include <qquickview.h>
#include <qqmlcontext.h>

#include <qsystemtrayicon.h>
#include <qmenu.h>
#include <qdir.h>
#include <qfont.h>
#include <qfontdatabase.h>
#include <qmessagebox.h>
#include <qdebug.h>

#include "../3rdparty/qtmintools/filewatcher/filewatcher.h"

#include "models/bangumilistmodel.h"
#include "models/sourcelinkmodel.h"
#include "models/rsssubscribemodel.h"
#include "models/bangumidatabaselistmodel.h"

#include "databasemodels/settingtbmodel.h"

#include "dao.h"
#include "entity/sqliteconfig.h"

#include "utils/mikanrssreader.h"

class CustomDbExceptionHandler : public DbExceptionHandler {
public:
    using DbExceptionHandler::DbExceptionHandler;

    void initDbFail(DbErrCode errcode, const QString& reason) override {
        QMessageBox::critical(nullptr, QStringLiteral("数据库错误[0]"), reason);
        qApp->exit();
    }

    void databaseOpenFail(DbErrCode errcode, const QString& failReason) override {
        QMessageBox::critical(nullptr, QStringLiteral("数据库错误[1]"), failReason);
        qApp->exit();
    }

    void execFail(DbErrCode errcode, const QString& lastErr) {
        qDebug() << "database execute fail:" + lastErr;
    }

    void execWarning(const QString& info) override {}
};

void SqlLogPrinter(const QString& sql, const QVariantList& values) {
#ifdef QT_DEBUG
    static QString error = "[DAO PRINT DEBUG] [sql]: %1, [values]: ";
    if (sql.startsWith("insert into ir_bangumidatabase")) {
        return;
    }
    qDebug() << error.arg(sql) << values;
#endif
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    a.setFont(QFont("Microsoft YaHei UI"));

    daoSetQueryLogPrinter(SqlLogPrinter);
    DbLoader::init(SqliteConfig(), new CustomDbExceptionHandler);

    qmlRegisterType<BangumiListModel>("BangumiListModel", 0, 1, "BangumiListModel");
    qmlRegisterType<SourceLinkModel>("SourceLinkModel", 0, 1, "SourceLinkModel");
    qmlRegisterType<QmlSettingDialog>("QmlSettingDialog", 0, 1, "QmlSettingDialog");
    qmlRegisterType<RssSubscribeModel>("RssSubscribeModel", 0, 1, "RssSubscribeModel");
    qmlRegisterType<BangumiDatabaseListModel>("BangumiDatabaseListModel", 0, 1, "BangumiDatabaseListModel");
    QmlSettingDialog::writeAutoStartDefault();

    QQuickView view;
#ifdef QT_DEBUG
    const QDir workDir(PROJECT_UI_PATH);
    const QUrl sourceFile = QUrl::fromLocalFile(workDir.filePath("main.qml"));
    view.setSource(sourceFile);
#else

#endif
    view.setFlag(Qt::FramelessWindowHint);
    view.setColor(Qt::transparent);

    view.rootContext()->setContextProperty("mainWindow", &view);
    auto rssReader = new MikanRssReader;
    view.rootContext()->setContextProperty("rssReader", rssReader);
    rssReader->start();

    view.show();

#ifdef QT_DEBUG
    FileWatcher fileWatcher([&](const QString&) {
        view.engine()->clearComponentCache();
        view.setSource(sourceFile);
    });
    fileWatcher.setDirectory(PROJECT_UI_PATH, "qml");
#endif

    QSystemTrayIcon systemTray(QIcon(":/log.ico"));
    systemTray.show();
    QObject::connect(&systemTray, &QSystemTrayIcon::activated, [&] (QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            view.show();
        }
    });

    QMenu menu;
    menu.addAction(QIcon(":/resource/ic_home.png"), QStringLiteral("主页"), &view, &QQuickView::show);
    menu.addAction(QIcon(":/resource/ic_remove.png"), QStringLiteral("退出"),  [&] {
        a.exit();
    });
    systemTray.setContextMenu(&menu);

    return a.exec();
}

