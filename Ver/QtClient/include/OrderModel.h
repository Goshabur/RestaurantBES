#pragma once

#include <QAbstractListModel>

#include "OrderList.h"

namespace restbes {

class OrderModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(restbes::OrderList *orderList READ getOrderList WRITE setOrderList NOTIFY orderListChanged);
public:
    explicit OrderModel(QObject *parent = nullptr);

    enum {
        IdRole = Qt::UserRole,
        StatusRole,
        DateRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant
    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] OrderList *getOrderList() const;

    void setOrderList(OrderList *list);

signals:
    void orderListChanged();

private slots:
    void updatePersistentIndexList();

private:
    OrderList *orderList = nullptr;
};

}
