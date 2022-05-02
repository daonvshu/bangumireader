#include "rsssubscribemodel.h"

#include "databasemodels/subscribemodel.h"

RssSubscribeModel::RssSubscribeModel(QObject* parent): QAbstractListModel(parent) {
    subscribeItems = SubScribeGroupsModel::getAllSubscribeInfo();
}

int RssSubscribeModel::rowCount(const QModelIndex& parent) const {
    return subscribeItems.size();
}

QVariant RssSubscribeModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(subscribeItems[index.row()]);
    }

    return QVariant();
}

void RssSubscribeModel::refreshAll() {
    beginResetModel();
    subscribeItems = SubScribeGroupsModel::getAllSubscribeInfo();
    endResetModel();
}
