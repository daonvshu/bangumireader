#pragma once

#include <QAbstractListModel>

class BangumiDatabase;

struct BangumiDatabaseItem {
    Q_GADGET

    Q_PROPERTY(bool yearRow MEMBER yearRow)
    Q_PROPERTY(bool monthRow MEMBER monthRow)
    Q_PROPERTY(QVariantList data MEMBER data)

public:
    bool yearRow = false;
    bool monthRow = false;

    QVariantList data;
};
Q_DECLARE_METATYPE(BangumiDatabaseItem)

class BangumiDatabaseListModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(bool syncStatus MEMBER syncStatus NOTIFY syncStatusChanged)
    Q_PROPERTY(QString filterKeywords READ getFilterKeywords WRITE setFilterKeywords NOTIFY filterKeywordsChanged)
    Q_PROPERTY(QString filterType READ getFilterType WRITE setFilterType NOTIFY filterTypeChanged)

public:
    BangumiDatabaseListModel(QObject* parent = nullptr);

    QString getFilterKeywords() const {
        return filterKeywords;
    }

    void setFilterKeywords(const QString& keywords);

    QString getFilterType() const {
        return filterType;
    }

    void setFilterType(const QString& type);

    int rowCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    Q_INVOKABLE void syncData();

signals:
    void syncStatusChanged();
    void syncMessageChanged(const QStringList& message);
    void filterKeywordsChanged();
    void filterTypeChanged();

private:
    QList<BangumiDatabaseItem> sourceData;
    bool syncStatus;

    QString filterKeywords;
    QString filterType;

    QStringList syncMessage;

private:
    void readLocalData();
};