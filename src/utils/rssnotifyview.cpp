#include "rssnotifyview.h"

#include <qqmlcontext.h>
#include <qdir.h>

#include "mikanrssreader.h"
#include "myquickview.h"

RssNotifyView::RssNotifyView(const QList<MikanNewRssItemInfo>& info, QWindow* parent)
    : QQuickView(parent)
{
    setFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setColor(Qt::transparent);

    rootContext()->setContextProperty("dataLinks", QVariant::fromValue(info));
    rootContext()->setContextProperty("mainView", this);

#ifdef QT_DEBUG
    const QDir workDir(PROJECT_UI_PATH);
    const QUrl sourceFile = QUrl::fromLocalFile(workDir.filePath("rssnotify.qml"));
    setSource(sourceFile);
#else
    setSource(QUrl("qrc:/ui/rssnotify.qml"));
#endif
}

void RssNotifyView::showView(const QList<MikanNewRssItemInfo>& info) {
    auto view = new RssNotifyView(info);
    view->show();
}

void RssNotifyView::loadTarget(int id, QString title) {
    MyQuickView::loadRssTarget(id, title);
}

bool RssNotifyView::event(QEvent* e) {
    if (e->type() == QEvent::Close) {
        deleteLater();
    }
    return QQuickView::event(e);
}