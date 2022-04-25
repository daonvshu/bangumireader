#include "sourcelinkmodel.h"

#include "HttpClient.h"

#include <qprocess.h>
#include <qfiledialog.h>
#include <qeventloop.h>
#include <QtConcurrent/QtConcurrent>

SourceLinkModel::SourceLinkModel(QObject *parent)
    : QAbstractListModel(parent)
{
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
    checkStatus.resize(linkData[groupName].size());
    checkStatus.fill(false);
    endResetModel();
}

int SourceLinkModel::rowCount(const QModelIndex& parent) const {
    return linkData[groupName].size();
}

QVariant SourceLinkModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(linkData[groupName].at(index.row()));
    }
    if (role == Qt::UserRole + 1) {
        return checkStatus[index.row()];
    }
    return QVariant();
}

bool SourceLinkModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role == Qt::UserRole + 1) {
        checkStatus[index.row()] = value.toBool();
        dataChanged(index, index, { role });
        return true;
    }
    if (role == Qt::UserRole + 2) {
        selectDirectory(index.row());
    }
    return false;
}

QHash<int, QByteArray> SourceLinkModel::roleNames() const {
    return {
        {Qt::UserRole + 1, "rowChecked"},
        {Qt::UserRole + 2, "download"},
        {Qt::DisplayRole, "display"},
    };
}

int SourceLinkModel::groupSize(const QString& groupName) const {
    return linkData[groupName].size();
}

void SourceLinkModel::downloadSelectedRowLinks() {
    QStringList links;
    for (int i = 0; i < checkStatus.size(); i++) {
        if (checkStatus[i]) {
            links << linkData[groupName][i].downloadUrl;
        }
    }
    if (links.isEmpty()) return;

    auto savePath = QFileDialog::getExistingDirectory(nullptr, QStringLiteral("选择保存路径"), ".");
    if (!savePath.isEmpty()) {
        downloadTargetTorrentLink(savePath, links);
    }
}

void SourceLinkModel::refreshTorrentLinks() {
    if (bangumiId == -1) {
        return;
    }
    //TODO:make reader single instance
    auto reader = new MikanRssReader;
    reader->readRssContent(bangumiId, [&] (const QMap<QString, QList<MikanTorrentLinkData>>& groupData) {
        beginResetModel();
        linkData = groupData;
        if (linkData.isEmpty()) {
            endResetModel();
            return;
        }
        groupName = linkData.keys().first();
        checkStatus.resize(linkData[groupName].size());
        checkStatus.fill(false);
        endResetModel();
        groupNamesChanged();
    });
}

void SourceLinkModel::selectDirectory(int dataRow) {
    auto savePath = QFileDialog::getExistingDirectory(nullptr, QStringLiteral("选择保存路径"), ".");
    if (!savePath.isEmpty()) {
        downloadTargetTorrentLink(savePath, { linkData[groupName][dataRow].downloadUrl });
    }
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

        QString lastFilePath;

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

            lastFilePath = savePath + link.mid(link.lastIndexOf("/"));
            QFile file(lastFilePath);
            if (file.exists()) {
                count--;
                downloadNext();
                return;
            }

            static AeaQt::HttpClient client;
            client.get(link)
                .download(lastFilePath)
                .onDownloadFileSuccess([=](QString filePath) {
                qDebug() << "download:" << link << "success!";
            })
                .onDownloadFileFailed([=](QString error) {
                qDebug() << "download:" << link << "fail!" << error;
            })
                .onFinished([&] {
                count--;
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

        lastFilePath.replace("/", "\\");
        QProcess::execute("explorer /select, \"" + lastFilePath + "\"");

        qDebug() << "download links task finished!";
        downloading = false;
        emit downloadStatusChanged();
    });
}
