#pragma once

#include <qquickview.h>

class MyQuickView : public QQuickView {
    Q_OBJECT

public:
    explicit MyQuickView(QWindow* parent = nullptr);

    static void create();
    static void currentViewReload();
    static void loadRssTarget(int id, QString title);

signals:
    void prepareToHide();

protected:
    bool event(QEvent* event) override;

private:
    static MyQuickView* holdView;

private:
    void reloadSource();
};