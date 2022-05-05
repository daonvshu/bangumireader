#pragma once

#include <qquickview.h>

class MyQuickView : public QQuickView {
    Q_OBJECT

public:
    explicit MyQuickView(QWindow* parent = nullptr);

    void reloadSource();

signals:
    void prepareToHide();

protected:
    bool event(QEvent* e) override;
};