#pragma once

#include "abstractrouterwidget.h"

#include "ui_bangumilistpage.h"


struct BangumiData;
class BangumiListPage : public AbstractRouterWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE BangumiListPage(const QVariant& data, QWidget* parent);
    ~BangumiListPage();

    void runRouterEvent(const QString& event, const QVariant& data) override;

private:
    Ui::BangumiListPage ui;

    static int typeId;

private:
    void requestBangumiData(const QString& year, const QString& season);
    void parseHtmlData(const QByteArray& data);
    void showBangumiList(const QMap<int, QList<BangumiData>>& data);
};
Q_DECLARE_METATYPE(BangumiListPage*)
