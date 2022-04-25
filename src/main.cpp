#include <qapplication.h>
#include <qqmlapplicationengine.h>
#include <qquickview.h>
#include <qqmlcontext.h>

#include <qsystemtrayicon.h>
#include <qmenu.h>
#include <qdir.h>
#include <qfont.h>
#include <qfontdatabase.h>

#include "../3rdparty/qtmintools/filewatcher/filewatcher.h"

#include "models/bangumilistmodel.h"
#include "models/sourcelinkmodel.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    a.setFont(QFont("Microsoft YaHei UI"));

    qmlRegisterType<BangumiListModel>("BangumiListModel", 0, 1, "BangumiListModel");
    qmlRegisterType<SourceLinkModel>("SourceLinkModel", 0, 1, "SourceLinkModel");

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

    QMenu menu;
    menu.addAction(u8"Ö÷Ò³", &view, &QQuickView::show);
    menu.addAction(u8"ÍË³ö", &view, &QQuickView::close);
    systemTray.setContextMenu(&menu);

    return a.exec();
}

