#include "bangumidatabaselistmodel.h"

#include "utils/bangumidatabasereader.h"
#include "databasemodels/bangumidatamodel.h"

#include <qdatetime.h>

BangumiDatabaseListModel::BangumiDatabaseListModel(QObject* parent)
    : QAbstractListModel(parent)
    , syncStatus(false)
{
    readLocalData();
    if (sourceData.isEmpty()) {
        syncData();
    }
}

void BangumiDatabaseListModel::setFilterKeywords(const QString& keywords) {
    filterKeywords = keywords;
    readLocalData();
}

void BangumiDatabaseListModel::setFilterType(const QString& type) {
    filterType = type;
    readLocalData();
}

int BangumiDatabaseListModel::rowCount(const QModelIndex& parent) const {
    return sourceData.size();
}

QVariant BangumiDatabaseListModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(sourceData[index.row()]);
    }
    return QVariant();
}

void BangumiDatabaseListModel::syncData() {
    static int lastStep = -1;
    syncMessage.clear();
    syncStatus = true;
    emit syncStatusChanged();

    auto bangumiReader = new BangumiDatabaseReader;
    connect(bangumiReader, &BangumiDatabaseReader::downloadStep, this, [&](const QString& step) {
        lastStep = 0;
        syncMessage << step;
        emit syncMessageChanged(syncMessage);
    });
    connect(bangumiReader, &BangumiDatabaseReader::downloadProgress, this, [&](const QString& percentage) {
        if (lastStep != 1) {
            syncMessage << percentage;
        } else {
            syncMessage.removeLast();
            syncMessage << percentage;
        }
        lastStep = 1;
        emit syncMessageChanged(syncMessage);
    });
    connect(bangumiReader, &BangumiDatabaseReader::downloadFailed, this, [&](const QString& reason) {
        lastStep = 2;
        syncMessage << QStringLiteral("同步失败：") + reason;
        emit syncMessageChanged(syncMessage);

        //syncStatus = false;
        //emit syncStatusChanged();
    });
    connect(bangumiReader, &BangumiDatabaseReader::downloadFinished, this, [&] {
        lastStep = 3;
        syncMessage << QStringLiteral("同步成功！");
        emit syncMessageChanged(syncMessage);

        syncStatus = false;
        emit syncStatusChanged();

        readLocalData();
    });
    bangumiReader->start();
}

void BangumiDatabaseListModel::readLocalData() {
    beginResetModel();
    sourceData.clear();

    int lastYear = -1;
    int lastMonth = -1;

    auto data = BangumiDataModel::getLocalBangumiDataAll(filterKeywords, filterType);
    for (const auto& d : data) {
        auto time = QDateTime::fromMSecsSinceEpoch(d.getBeginDate());
        if (lastYear != time.date().year()) {
            lastYear = time.date().year();
            lastMonth = -1;
            sourceData.append(BangumiDatabaseItem{true, false, {QStringLiteral("%1年").arg(lastYear)}});
        }
        if (lastMonth != time.date().month()) {
            lastMonth = time.date().month();
            sourceData.append(BangumiDatabaseItem{ false, true, {QStringLiteral("%1月").arg(lastMonth)} });
        }
        if (sourceData.last().yearRow || sourceData.last().monthRow) {
            sourceData.append(BangumiDatabaseItem{ false, false , {} });
        }
        sourceData.last().data.append(QVariant::fromValue(d));
    }

    endResetModel();
}
