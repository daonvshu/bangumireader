#include "sourcelinkmodel.h"

#include "HttpClient.h"

#include <qprocess.h>
#include <qfiledialog.h>
#include <qeventloop.h>
#include <QtConcurrent/QtConcurrent>
#include <ShlObj.h>

#include "ConnectionPool.h"
#include "databasemodels/remarktbmodel.h"
#include "databasemodels/subscribemodel.h"
#include "databasemodels/savepathcachemodel.h"

#include "utils/textutil.h"

SourceLinkModel::SourceLinkModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &SourceLinkModel::requestRefreshList, this, [&] {
        beginResetModel();
        endResetModel();
    });
}

void SourceLinkModel::setBangumiId(int id) {
    this->bangumiId = id;
    refreshTorrentLinks();
}

QStringList SourceLinkModel::getGroupNames() const {
    return linkData.keys();
}

void SourceLinkModel::setGroupName(const QString& name) {
    beginResetModel();
    groupName = name;
    selectedGroupNameChanged();
    endResetModel();
}

void SourceLinkModel::setFilterKeywords(const QString& keywords) {
    beginResetModel();
    this->filterKeywords = keywords;
    selectedGroupNameChanged();
    endResetModel();
}

int SourceLinkModel::rowCount(const QModelIndex& parent) const {
    return filterData.size();
}

QVariant SourceLinkModel::data(const QModelIndex& index, int role) const {
    if (role == RoleDisplay) {
        return QVariant::fromValue(filterData.at(index.row()));
    }
    if (role == RoleRowChecked) {
        return checkStatus[index.row()];
    }
    if (role == RoleDownloaded) {
        return RemarkTbModel::checkLinkExist(filterData.at(index.row()).downloadUrl);
    }
    if (role == RoleNewStatus) {
        return SubScribeGroupsModel::isSubscribeGroupItemNew(/*SubScribeGroupsModel::getGroup(bangumiId, groupName).getId(), */filterData.at(index.row()).downloadUrl);
    }
    return QVariant();
}

bool SourceLinkModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role == RoleRowChecked) {
        checkStatus[index.row()] = value.toBool();
        dataChanged(index, index, { role });
        return true;
    }
    if (role == RoleToDownload) {
        selectDirectory(index.row());
        return true;
    }
    if (role == RoleNewStatus) {
        removeNewStatus(index.row());
        dataChanged(index, index, { RoleNewStatus });
        return true;
    }
    return false;
}

QHash<int, QByteArray> SourceLinkModel::roleNames() const {
    return {
        {RoleRowChecked, "rowChecked"},
        {RoleToDownload, "download"},
        {RoleDisplay, "display"},
        {RoleDownloaded, "downloaded"},
        {RoleNewStatus, "newStatus"},
    };
}

void SourceLinkModel::downloadSelectedRowLinks() {
    QStringList links;
    for (int i = 0; i < checkStatus.size(); i++) {
        if (checkStatus[i]) {
            links << filterData[i].downloadUrl;
            removeNewStatus(i);
        }
    }
    if (links.isEmpty()) return;

    getSavedDirectory([=] (const QString& savePath) {
        downloadTargetTorrentLink(savePath, links);
    });
}

void SourceLinkModel::refreshTorrentLinks() {
    if (bangumiId == -1) {
        return;
    }
    MikanRssReader::readRssContent(bangumiId, this, &SourceLinkModel::solveRssContentData);
}

void SourceLinkModel::solveRssContentData(const QMap<QString, QList<MikanTorrentLinkData>>& groupData) {
    beginResetModel();
    linkData = groupData;
    filterData.clear();
    if (linkData.isEmpty()) {
        endResetModel();
        return;
    }
    groupName = linkData.keys().first();
    selectedGroupNameChanged();
    endResetModel();
    groupNamesChanged();
}

void SourceLinkModel::selectDirectory(int dataRow) {
    getSavedDirectory([&, dataRow] (const QString& savePath) {
        downloadTargetTorrentLink(savePath, { filterData[dataRow].downloadUrl });
        removeNewStatus(dataRow);
    });
}

int SourceLinkModel::groupSize(const QString& groupName) const {
    return linkData[groupName].size();
}

void SourceLinkModel::selectAllItems() {
    bool allIsChecked = true;
    for (const auto& check: checkStatus) {
        allIsChecked = allIsChecked && check;
    }
    for (auto& check: checkStatus) {
        check = !allIsChecked;
    }

    dataChanged(index(0), index(checkStatus.size() - 1), { RoleRowChecked });
}

//5 limit parallel download task
void SourceLinkModel::downloadTargetTorrentLink(const QString& savePath, const QStringList& links) {

    if (links.isEmpty()) return;

    downloading = true;
    emit downloadStatusChanged();

    QtConcurrent::run([=] {

        QStringList prepareLinks = links;
        QEventLoop loop;

        int count = 0;

        std::function<void()> downloadNext;
        std::function<void(const QString& link)> downloadTask;
        std::function<void()> doFinished;

        QStringList downloadedFiles;

        downloadNext = [&] {
            if (!prepareLinks.isEmpty()) {
                downloadTask(prepareLinks.takeFirst());
                if (count < 5) {
                    downloadNext();
                }
                return;
            }
            if (count == 0) {
                doFinished();
            }
        };

        downloadTask = [&] (const QString& link) {
            count++;

            auto lastFilePath = savePath + link.mid(link.lastIndexOf("/"));
            QFile file(lastFilePath);
            if (file.exists()) {
                downloadedFiles << lastFilePath;
                count--;
                downloadNext();
                return;
            }

            static AeaQt::HttpClient client;
            client.get(link)
                .download(lastFilePath)
                .onDownloadFileSuccess([&, link](QString filePath) {
                qDebug() << "download:" << link << "success!";
                downloadedFiles << filePath;
            })
                .onDownloadFileFailed([=](QString error) {
                qDebug() << "download:" << link << "fail!" << error;
            })
                .onFinished([&, link] {
                count--;
                RemarkTbModel::cacheNewDownloadedLink(link);
                downloadNext();
            })
                .exec();
        };

        doFinished = [&] {
            loop.exit();
        };

        downloadNext();
        if (count != 0) {
            loop.exec();
        }

        if (!downloadedFiles.isEmpty()) {
            int fileCount = downloadedFiles.size();
            ITEMIDLIST** fileItems = new ITEMIDLIST * [fileCount];
            for (int p = 0; p < fileCount; p++) {
                auto path = downloadedFiles[p].replace("/", "\\");
                fileItems[p] = ILCreateFromPathW(path.toStdWString().c_str());
            }
            auto savePathRel = savePath;
            savePathRel.replace("/", "\\");
            SHOpenFolderAndSelectItems(ILCreateFromPathW(savePathRel.toStdWString().c_str()), fileCount, (LPCITEMIDLIST*)fileItems, 0);
            delete[] fileItems;
        }

        qDebug() << "download links task finished!";
        downloading = false;
        emit downloadStatusChanged();
        emit requestRefreshList();

        ConnectionPool::closeConnection();
    });
}

QVariant SourceLinkModel::getCurrentSubscribeGroup() const {
    auto data = SubScribeGroupsModel::getGroup(bangumiId, groupName);
    data.setBangumiId(bangumiId);
    data.setGroupName(groupName);
    data.__putExtra("rssLink", MikanRssReader::rssLink(bangumiId));
    return QVariant::fromValue(data);
}

void SourceLinkModel::saveSubscribe(const QString& title, const QString& keywords) {
    auto data = getCurrentSubscribeGroup().value<SubscribeGroups>();
    data.__putExtra("title", title);
    data.setKeywords(keywords);
    SubScribeGroupsModel::updateSubscribeGroup(data);

    //insert items
    //SubScribeGroupsModel::removeSubscribeGroupItems(data.getId());

    QStringList torrentLinks;
    QList<MikanTorrentLinkData> currentFilterData;
    try {
        currentFilterData = TextUtils::filterByKeywords<MikanTorrentLinkData>(data.getKeywords(), linkData[groupName], [&](const MikanTorrentLinkData& d) {
            return d.title;
        });
    } catch (TextParseException&) {
        currentFilterData = linkData[groupName];
    }
    auto existItems = SubScribeGroupsModel::getAllSubscribeGroupItems(data.getId());
    QStringList torrentLinkAll;
    for (const auto& d: currentFilterData) {
        bool exist = false;
        for (const auto& item: existItems) {
            if (item.getSourceLink() == d.downloadUrl) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            torrentLinks << d.downloadUrl;
        }
        torrentLinkAll << d.downloadUrl;
    }
    QList<int> prepareRemoveIds;
    for (const auto& item: existItems) {
        if (!torrentLinkAll.contains(item.getSourceLink())) {
            prepareRemoveIds << item.getId();
        }
    }
    if (!prepareRemoveIds.isEmpty()) {
        SubScribeGroupsModel::removeTargetSubscribeGroupItems(prepareRemoveIds);
    }
    SubScribeGroupsModel::insertSubscribeGroupItems(data.getId(), torrentLinks);
    emit dataChanged(index(0), index(filterData.size() - 1), { RoleNewStatus });

    if (!groupSubscribed) {
        groupSubscribed = true;
        emit groupSubscribedChanged();
    }
}

void SourceLinkModel::removeSubscribe(const QVariant& group) {
    auto data = group.value<SubscribeGroups>();
    if (data.getId() == -1) {
        return;
    }
    SubScribeGroupsModel::removeSubscribeGroup(data);

    SubScribeGroupsModel::removeSubscribeGroupItems(data.getId());
    emit dataChanged(index(0), index(filterData.size() - 1), { RoleNewStatus });

    groupSubscribed = false;
    emit groupSubscribedChanged();
}

void SourceLinkModel::reloadFilterLinkData() {
    filterData.clear();

    try {
        filterData = TextUtils::filterByKeywords<MikanTorrentLinkData>(filterKeywords, linkData[groupName], [&](const MikanTorrentLinkData& d) {
            return d.title;
        });
    } catch (TextParseException&) {
        filterData << linkData[groupName];
    }
}

void SourceLinkModel::selectedGroupNameChanged() {
    reloadFilterLinkData();
    checkStatus.resize(filterData.size());
    checkStatus.fill(false);

    groupSubscribed = SubScribeGroupsModel::getGroup(bangumiId, groupName).getId() != -1;
    emit groupSubscribedChanged();
}

void SourceLinkModel::removeNewStatus(int row) {
    SubScribeGroupsModel::removeSubscribeGroupItemNewStatus(filterData[row].downloadUrl);
}

void SourceLinkModel::getSavedDirectory(const std::function<void(const QString&)>& callback) const {
    auto savePath = QFileDialog::getExistingDirectory(nullptr, QStringLiteral("Ñ¡Ôñ±£´æÂ·¾¶"), SavePathCacheModel::getLastSaveDirectory(bangumiId));
    if (!savePath.isEmpty()) {
        QDir saveDir(savePath);
        auto currentBangumiTitle = SubScribeGroupsModel::getSubscribeTargetTitle(bangumiId);
        auto validTitle = currentBangumiTitle.remove(QRegExp("[<>:\"/\\\\\\|\\?\\*]"));
        if (saveDir.dirName() != validTitle) {
            if (saveDir.mkpath(savePath + "/" + validTitle)) {
                saveDir.cd(validTitle);
                savePath = saveDir.absolutePath();
            }
        }

        SavePathCacheModel::updateSaveDirectory(bangumiId, savePath);
        callback(savePath);
    }
}
