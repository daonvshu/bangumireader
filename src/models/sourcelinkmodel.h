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
    Q_PROPERTY(QString filterKeywords READ getFilterKeywords WRITE setFilterKeywords NOTIFY filterKeywordsChanged)
    Q_PROPERTY(bool groupSubscribed READ isGroupSubscribed NOTIFY groupSubscribedChanged)

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

    bool isGroupSubscribed() const {
        return groupSubscribed;
    }

    Q_INVOKABLE int groupSize(const QString& groupName) const;
    Q_INVOKABLE void selectAllItems();
    Q_INVOKABLE void downloadSelectedRowLinks();

    Q_INVOKABLE QVariant getCurrentSubscribeGroup() const;
    Q_INVOKABLE void saveSubscribe(const QString& title, const QString& keywords);
    Q_INVOKABLE void removeSubscribe(const QVariant& group);

signals:
    void groupNamesChanged();
    void downloadStatusChanged();
    void requestRefreshList();
    void filterKeywordsChanged();
    void groupSubscribedChanged();

private:
    int bangumiId = -1;
    QString groupName;
    bool downloading = false;
    QString filterKeywords;
    bool groupSubscribed = false;

    QMap<QString, QList<MikanTorrentLinkData>> linkData;
    QList<MikanTorrentLinkData> filterData;
    QVector<bool> checkStatus;

    enum {
        RoleDisplay = Qt::DisplayRole,
        RoleRowChecked = Qt::UserRole + 1,
        RoleToDownload,
        RoleDownloaded,
        RoleNewStatus,
    };

private:
    void refreshTorrentLinks();
    void solveRssContentData(const QMap<QString, QList<MikanTorrentLinkData>>& groupData);
    void selectDirectory(int dataRow);
    void downloadTargetTorrentLink(const QString& savePath, const QStringList& links);
    void reloadFilterLinkData();
    void selectedGroupNameChanged();
    void removeNewStatus(int row);
};
