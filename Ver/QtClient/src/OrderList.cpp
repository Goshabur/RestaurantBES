#include "OrderList.h"

namespace restbes {

OrderList::OrderList(QObject *parent) : QObject(parent) {
}

int OrderList::size() const {
    return orderData.rlock()->size();
}

OrderItem OrderList::getItemAt(int index) const {
    if (index < 0 || index >= size()) return {};
    return orderData.rlock()->at(index);
}

int OrderList::getIndex(int id) const {
    auto lockedIndexes = indexes.rlock();
    if (lockedIndexes->count(id) == 0) return -1;
    return lockedIndexes->at(id);
}

int OrderList::getId(int index) const {
    return getItemAt(index).order_id;
}

void OrderList::setItemStatus(int id, int value, unsigned int date) {
    if (indexes.rlock()->count(id)) {
        if (getItemStatus(id) == value) return;
        orderData.wlock()->at(getIndex(id)).status = value;
        emit itemChanged(id);
    } else {
        emit beginInsertItem(size());
        indexes.wlock()->insert({id, size()});
        orderData.wlock()->push_back({id, value, date});
        emit endInsertItem();
    }
}

void OrderList::setOrderData(OrderData newData) {
    emit beginChangeLayout();
    auto lockedOrderData = orderData.wlock();
    auto lockedIndexes = indexes.wlock();
    *lockedOrderData = std::move(newData);
    lockedIndexes->clear();
    int i = 0;
    for (const auto &item: *lockedOrderData) {
        lockedIndexes->operator[](item.order_id) = i++;
    }
    emit endChangeLayout();
}

int OrderList::getItemStatus(int id) const {
    return getItem(id).status;
}

OrderData::const_iterator OrderList::begin() const {
    return orderData.rlock()->begin();
}

OrderData::const_iterator OrderList::end() const {
    return orderData.rlock()->end();
}

OrderItem OrderList::getItem(int id) const {
    return getItemAt(getIndex(id));
}

unsigned int OrderList::getTimestamp() const {
    return timestamp;
}

void OrderList::setTimestamp(unsigned int newTimestamp) {
    timestamp = newTimestamp;
}

}