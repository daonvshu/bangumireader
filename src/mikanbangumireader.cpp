#include "mikanbangumireader.h"

#include <qmenu.h>
#include <qsystemtrayicon.h>

#include "qfonticon.h"
#include "qrouter.h"

MikanBangumiReader::MikanBangumiReader(QWidget* parent)
    : ShadowWidget(parent)
{
    setupUi(ui);
    setDraggableTitleHeight(36);

    ui.qprogress_wait->setVisible(false);

    auto systemTray = new QSystemTrayIcon(QIcon(":/log.ico"), this);
    systemTray->show();

    menu = new QMenu(this);
    menu->addAction(u8"主页", this, &MikanBangumiReader::show);
    menu->addAction(u8"退出", this, &MikanBangumiReader::close);
    systemTray->setContextMenu(menu);

    QRouter::install(ui.stackedWidget);
    QRouter::of().push("BangumiListPage");

    initSelector();
    selectorChanged();
}

void MikanBangumiReader::initSelector() const {
    int nowYear = QDate::currentDate().year();
    for (int i = 2013; i <= nowYear; i++) {
        ui.year_selector->items << QString::number(i);
    }
    ui.year_selector->curText = QString::number(nowYear);

    ui.year_selector->curTextChangeEvt.add(this, &MikanBangumiReader::selectorChanged);
    ui.season_selector->curTextChangeEvt.add(this, &MikanBangumiReader::selectorChanged);
}

void MikanBangumiReader::selectorChanged() const {
    QRouter::of().sendEventTo("BangumiListPage", "reload",
        QVariant::fromValue(qMakePair(ui.year_selector->curText(), ui.season_selector->curText())));
}
