#include "myquickview.h"

#include <qdir.h>
#include <qqmlapplicationengine.h>
#include <qqmlcontext.h>
#include <qevent.h>

MyQuickView* MyQuickView::holdView = nullptr;

MyQuickView::MyQuickView(QWindow* parent)
    : QQuickView(parent)
{
    setFlag(Qt::FramelessWindowHint);
    setColor(Qt::transparent);

    rootContext()->setContextProperty("mainWindow", this);
    rootContext()->setContextProperty("appVersion", APP_VERSION);

    connect(this, &MyQuickView::prepareToHide, this, &QQuickView::close, Qt::QueuedConnection);

    reloadSource();
}

void MyQuickView::create() {
    if (holdView != nullptr) {
        holdView->requestActivate();
        return;
    }
    holdView = new MyQuickView;
    holdView->show();
}

void MyQuickView::currentViewReload() {
    if (holdView == nullptr) {
        return;
    }
    holdView->reloadSource();
}

void MyQuickView::loadRssTarget(int id, QString title) {
    if (holdView == nullptr) {
        create();
    }
    QMetaObject::invokeMethod((QObject*)holdView->rootObject(), "loadRssTarget", Q_ARG(QVariant, QVariant(id)), Q_ARG(QVariant, QVariant(title)));
}

bool MyQuickView::event(QEvent* e) {
    if (e->type() == QEvent::Close) {
        holdView->deleteLater();
        holdView = nullptr;
    }
    return QQuickView::event(e);
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