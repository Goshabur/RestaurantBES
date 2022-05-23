#include "OrderModel.h"
#include "OrderList.h"

#define REFERENCE_CONCAT(x, y) & x :: y

#define CONNECT_SIGNALS(source, sourceClass) \
connect(source, REFERENCE_CONCAT(sourceClass,beginInsertItem), this, [=](int index) { \
    beginInsertRows(QModelIndex(), index, index); \
}); \
connect(source, REFERENCE_CONCAT(sourceClass,endInsertItem), this, [=]() { \
    endInsertRows(); \
}); \
connect(source, REFERENCE_CONCAT(sourceClass,beginRemoveItem), this, [=](int index) { \
    beginRemoveRows(QModelIndex(), index, index); \
});                                         \
connect(source, REFERENCE_CONCAT(sourceClass,endRemoveItem), this, [=]() { \
    endRemoveRows(); \
}); \
connect(source, REFERENCE_CONCAT(sourceClass,beginChangeLayout), this, [=]() { \
    layoutAboutToBeChanged(); \
}); \
connect(source, REFERENCE_CONCAT(sourceClass,endChangeLayout),this, [=]() {           \
    updatePersistentIndexList();                                         \
    layoutChanged(); \
})

namespace restbes {

OrderModel::OrderModel(QObject *parent) : QAbstractListModel(parent) {

}

int OrderModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid() || !orderList)
        return 0;

    return orderList->size();
}

QVariant OrderModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !orderList)
        return {};

    const OrderItem item = orderList->getItemAt(index.row());
    switch (role) {
        case IdRole:
            return {item.order_id};
        case StatusRole:
            return {item.status};
        default:
            return {};
    }
}

QHash<int, QByteArray> OrderModel::roleNames() const {
    static QHash<int, QByteArray> names{
            {IdRole,     "id"},
            {StatusRole, "status"}
    };
    return names;
}

OrderList *OrderModel::getOrderList() const {
    return orderList;
}

void OrderModel::setOrderList(OrderList *list) {
    beginResetModel();

    if (orderList)
        orderList->disconnect(this);

    orderList = list;

    if (orderList) {
        CONNECT_SIGNALS(orderList, OrderList);
        connect(orderList,
                &OrderList::itemStatusChanged,
                this,
                [=](int id) {
                    auto ind = index(orderList->getIndex(id), 0);
                    dataChanged(ind, ind, QVector<int>{StatusRole});
                });
    }

    endResetModel();
}

void OrderModel::updatePersistentIndexList() {
    QModelIndexList newList{};
    newList.reserve(orderList->size());
    int ind = 0;
    while (ind < orderList->size()) {
        newList.push_back(index(ind++, 0));
    }
    changePersistentIndexList(persistentIndexList(), newList);
}


}
