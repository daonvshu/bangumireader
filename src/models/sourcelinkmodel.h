#pragma once

#include <QAbstractListModel>

#include "utils/mikanrssreader.h"

class SourceLinkModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int bangumiId READ getBangumiId WRITE setBangumiId)
    Q_PROPERTY(QString groupName READ getGroupName WRITE setGroupName)
    Q_PROPERTY(QStringList groupNames READ getGroupNames NOTIFY groupNamesChanged)
    Q_PROPERTY(bool downloading READ getDownloadingStatus NOTIFY downloadStatusChanged)
    Q_PROPERTY(QString filterKeywords READ getFilterKeywords WRITE setFilterKeywords)

public:
    SourceLinkModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    int getBangumiId() const {
        return bangumiId;
    }

    void setBangumiId(int id);

    QString getGroupName() const {
        return groupName;
    }

    void setGroupName(const QString& name);

    QStringList getGroupNames() const;

    bool getDownloadingStatus() const {
        return downloading;
    }

    QString getFilterKeywords() const {
        return filterKeywords;
    }

    void setFilterKeywords(const QString& keywords);

    Q_INVOKABLE int groupSize(const QString& groupName) const;
    Q_INVOKABLE void selectAllItems();
    Q_INVOKABLE void downloadSelectedRowLinks();

signals:
    void groupNamesChanged();
    void downloadStatusChanged();
    void requestRefreshList();

private:
    int bangumiId = -1;
    QString groupName;
    bool downloading = false;
    QString filterKeywords;

    QMap<QString, QList<MikanTorrentLinkData>> linkData;
    QList<MikanTorrentLinkData> filterData;
    QVector<bool> checkStatus;

private:
    void refreshTorrentLinks();
    void selectDirectory(int dataRow);
    void downloadTargetTorrentLink(const QString& savePath, const QStringList& links);
    void reloadFilterLinkData();
    void selectedGroupNameChanged();
};
