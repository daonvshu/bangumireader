#pragma once

#include <qsystemtrayicon.h>
#include <qwidget.h>

#include "ShadowWidget.h"

#include "ui_mikanbangumireader.h"

class MikanBangumiReader : public ShadowWidget {
public:
    explicit MikanBangumiReader(QWidget* parent);

private:
    void initSelector() const;
    void selectorChanged() const;

private:
    QMenu* menu;
    Ui::MikanBangumiReader ui;
};