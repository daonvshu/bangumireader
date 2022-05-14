#pragma once

#include <qquickview.h>

struct MikanNewRssItemInfo;

class RssNotifyView: public QQuickView {
    Q_OBJECT

public:
    explicit RssNotifyView(const QList<MikanNewRssItemInfo>& info, QWindow* parent = nullptr);

    static void showView(const QList<MikanNewRssItemInfo>& info);

    Q_INVOKABLE static void loadTarget(int id, QString title);

protected:
    bool event(QEvent* event) override;
};
