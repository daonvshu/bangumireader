#include "bangumilistpage.h"

#include "HttpClient.h"
#include "SmoothScrollbar.h"

#include <qxmlstream.h>
#include <qdebug.h>
#include <qlistview.h>
#include <qscroller.h>
#include <qstandarditemmodel.h>

#include "ui_bangumilistitem.h"

class BangumiListItem : public QWidget {
public:
    BangumiListItem(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        ui.setupUi(this);
    }

    Ui::BangumiListItem ui;

protected:
    void resizeEvent(QResizeEvent*) override {
        auto text = ui.label_title->text();
        int w = fontMetrics().width(text);
        if (w > ui.label_title->width()) {
            auto elided = fontMetrics().elidedText(text, Qt::ElideMiddle, ui.label_title->width());
            ui.label_title->text = elided;
        }
    }
};

int BangumiListPage::typeId = qRegisterMetaType<BangumiListPage*>();
BangumiListPage::BangumiListPage(const QVariant& data, QWidget* parent)
    : AbstractRouterWidget(data, parent)
{
    ui.setupUi(this);
    //ui.scrollArea->setVerticalScrollBar(new SmoothScrollbar(this));
    QScroller::grabGesture(ui.scrollArea, QScroller::LeftMouseButtonGesture);
}

BangumiListPage::~BangumiListPage()
{
}

void BangumiListPage::runRouterEvent(const QString& event, const QVariant& data) {
    if (event == "reload") {
        auto d = data.value<QPair<QString, QString>>();
        requestBangumiData(d.first, d.second);
    }
}

#define HOST_URL            "https://mikanani.me"
#define BANGUMI_REQUEST_URL QString(HOST_URL ## "/Home/BangumiCoverFlowByDayOfWeek?year=%1&seasonStr=%2")

void BangumiListPage::requestBangumiData(const QString& year, const QString& season) {
    static AeaQt::HttpClient client;
    client.setNetworkAccessible(QNetworkAccessManager::Accessible);
    client.get(BANGUMI_REQUEST_URL.arg(year).arg(season))
        .onFailed([&] (const QByteArray& reason) {
            qDebug() << reason;
        })
        .onSuccess([&](const QByteArray& data) {
            parseHtmlData(data);
        })
        .onFinished([&] {

        })
    .exec();
}

struct BangumiData {
    QString title;
    QString thumbnail;
    QString link;

    QString toString() const {
        return QString("[title:%1, thumbnail:%2, link:%3]").arg(title, thumbnail, link);
    }
};

void BangumiListPage::parseHtmlData(const QByteArray& data) {
    QXmlStreamReader reader("<div>" + data + "</div>");

    QMap<int, QList<BangumiData>> bangumiData;

    BangumiData currentBangumi;
    bool itemBegin = false;
    int currentWeek = -1;

    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == "div") {
                auto weekStr = reader.attributes().value("data-dayofweek");
                if (!weekStr.isEmpty()) {
                    currentWeek = weekStr.toInt();
                    bangumiData.insert(currentWeek, {});
                }
            }
            if (reader.name() == "li") {
                itemBegin = true;
                currentBangumi = BangumiData();
            }
            if (itemBegin) {
                if (reader.name() == "span") {
                    currentBangumi.thumbnail = reader.attributes().value("data-src").toString();
                } else if (reader.name() == "a") {
                    currentBangumi.link = reader.attributes().value("href").toString();
                    currentBangumi.title = reader.attributes().value("title").toString();
                } else if (reader.name() == "div" && currentBangumi.title.isEmpty()) {
                    auto title = reader.attributes().value("title");
                    if (!title.isEmpty()) {
                        currentBangumi.title = reader.attributes().value("title").toString();
                    }
                }
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (reader.name() == "li") {
                itemBegin = false;
                bangumiData[currentWeek] << currentBangumi;
            }
        }
    }
    if (reader.hasError()) {
        qDebug() << "reader error occur:" << reader.error() << reader.errorString();
    }
    qDebug() << "decode bangumi data size:" << bangumiData.size();
    for (const auto& w : bangumiData.keys()) {
        qDebug() << "week:" << w;
        for (const auto& d : bangumiData[w]) {
            qDebug() << d.toString();
        }
    }
    showBangumiList(bangumiData);
}

void BangumiListPage::showBangumiList(const QMap<int, QList<BangumiData>>& data) {
    //clear previous data
    {
        QLayoutItem* item;
        while ((item = ui.listcontent->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    const auto& createWeekLabel = [&] (int week, QWidget* parent) {
        auto label = new QLabelEx(parent);
        int width = ui.listcontent->parentWidget()->width();
        label->setMinimumHeight(24);
        label->setStyleSheet(QString("background-color: qlineargradient("
                                "spread:repeat, x1:0, y1:0, x2:0.02, y2:%2," 
                                "stop:0 #FF73402A,"
                                "stop:0.5 #FF73402A,"
                                "stop:0.50002 #FF7A4C39," 
                                "stop:1 #FF7A4C39"
                                ");"
            "border-radius:6px;color:white;padding-left:12px;").arg(QString::number(0.02 * 24 / width, 'f', 4)));
        if (week < 7) {
            static auto labelWeeks = QStringList() << u8"日" << u8"一" << u8"二" << u8"三" << u8"四" << u8"五" << u8"六";
            label->text = u8"星期" + labelWeeks[week];
        } else if (week == 7) {
            label->text = u8"剧场版";
        } else if (week == 8) {
            label->text = u8"OVA";
        }
        return label;
    };

    static int rowSize = 5;

    const auto& createItem = [&](const BangumiData& d, int spacing, QWidget* parent) {
        auto item = new BangumiListItem(parent);
        int width = (ui.listcontent->parentWidget()->width() - (rowSize - 1) * spacing) / rowSize;
        item->setFixedSize(width, width);
        if (!d.thumbnail.isEmpty()) {
            item->ui.label_thumbnail->image.targetW(width).network(HOST_URL + d.thumbnail);
        }
        item->ui.label_title->text = d.title;
        return item;
    };

    if (data.isEmpty()) {
        return;
    }

    for (int i = 0; i < 7; i++) {
        const auto& items = data[i];

        ui.listcontent->addWidget(createWeekLabel(i, ui.listcontent->parentWidget()));

        int index = -1;
        QWidget* rowWidget = nullptr;
        QHBoxLayout* hlayout = nullptr;
        for (const auto& item : items) {
            if (++index % rowSize == 0) {
                rowWidget = new QWidget(ui.listcontent->parentWidget());
                hlayout = new QHBoxLayout(rowWidget);
                hlayout->setContentsMargins(0, 0, 0, 0);
                ui.listcontent->addWidget(rowWidget);
            } 
            hlayout->addWidget(createItem(item, hlayout->spacing(), rowWidget));
        }
        if (items.size() % rowSize != 0) {
            hlayout->addSpacerItem(new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
        }
    }
}
