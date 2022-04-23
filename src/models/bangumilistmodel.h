#pragma once

#include <QAbstractListModel>

struct BangumiData {
    Q_GADGET

    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString thumbnail MEMBER thumbnail)
    Q_PROPERTY(QString link MEMBER link)

public:
    QString title;
    QString thumbnail;
    QString link;

    QString toString() const {
        return QString("{\"title\":\"%1\", \"thumbnail\" : \"%2\", \"link\" : \"%3\"}, )").arg(title, thumbnail, link);
    }
};
Q_DECLARE_METATYPE(BangumiData)

class BangumiListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString year READ getYear WRITE setYear)
    Q_PROPERTY(QString season READ getSeason WRITE setSeason)

public:
    BangumiListModel(QObject *parent = nullptr);

    QString getYear() const {
        return selectedYear;
    }

    void setYear(const QString& year) {
        selectedYear = year;
        refreshData();
    }

    QString getSeason() const {
        return selectedSeason;
    }

    void setSeason(const QString& season) {
        selectedSeason = season;
        refreshData();
    }

    int rowCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    enum {
        RoleHeader = Qt::UserRole + 1,
        RoleData,
        RoleType,
    };

private:
    QString selectedYear, selectedSeason;

    QMap<int, QList<BangumiData>> bangumiData;
    QVariantList formattedData;

private:
    void refreshData();
    void parseHtmlData(const QByteArray& data);
};
