#include "myquickview.h"

#include <qdir.h>
#include <qqmlapplicationengine.h>
#include <qqmlcontext.h>
#include <qevent.h>

#include "mikanrssreader.h"

MyQuickView::MyQuickView(QWindow* parent)
    : QQuickView(parent)
{
    setFlag(Qt::FramelessWindowHint);
    setColor(Qt::transparent);

    rootContext()->setContextProperty("mainWindow", this);
    rootContext()->setContextProperty("appVersion", APP_VERSION);

    auto rssReader = new MikanRssReader;
    rootContext()->setContextProperty("rssReader", rssReader);
    rssReader->start();

    connect(this, &MyQuickView::prepareToHide, this, &QQuickView::hide, Qt::QueuedConnection);

    reloadSource();
}

void MyQuickView::reloadSource() {
    engine()->clearComponentCache();
#ifdef QT_DEBUG
    const QDir workDir(PROJECT_UI_PATH);
    const QUrl sourceFile = QUrl::fromLocalFile(workDir.filePath("main.qml"));
    setSource(sourceFile);
#else
    setSource(QUrl("qrc:/ui/main.qml"));
#endif
}

bool MyQuickView::event(QEvent* e) {
    if (e->type() == QEvent::Close) {
        hide();
        e->ignore();
        return true;
    }
    if (e->type() == QEvent::Hide) {
        setSource(QUrl());
        engine()->clearComponentCache();
        releaseResources();
        return true;
    }
    if (e->type() == QEvent::Show) {
        static bool firstShow = true;
        if (firstShow) { //ignore first load
            firstShow = false;
        } else {
            reloadSource();
            raise();
            return true;
        }
    }
    return QQuickView::event(e);
}
