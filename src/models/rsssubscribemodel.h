#pragma once

#include <QAbstractListModel>

#include "entity/SubscribeTb.h"

class RssSubscribeModel : public QAbstractListModel {
    Q_OBJECT

public:
    RssSubscribeModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    Q_INVOKABLE void refreshAll();

private:
    SubscribeTbList subscribeItems;
};