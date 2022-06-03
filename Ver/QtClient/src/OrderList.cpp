#include "OrderList.h"

namespace restbes {

OrderList::OrderList(QObject *parent) : QObject(parent) {
}

int OrderList::size() const {
    return orderData->size();
}

OrderItem OrderList::getItemAt(int index) const {
    if (index < 0 || index >= size()) return {};
    return orderData->at(index);
}

int OrderList::getIndex(int id) const {
    if (indexes.count(id) == 0) return -1;
    return indexes.at(id);
}

int OrderList::getId(int index) const {
    return getItemAt(index).order_id;
}

void OrderList::setItemStatus(int id, int value, unsigned int date) {
    if (indexes.count(id)) {
        if (getItemStatus(id) == value) return;
        orderData->at(getIndex(id)).status = value;
        emit itemChanged(id);
    } else {
        emit beginInsertItem(size());
        indexes.insert({id, size()});
        orderData->push_back({id, value, date});
        emit endInsertItem();
    }
}

void OrderList::setOrderData(std::shared_ptr<OrderData> newData) {
    emit beginChangeLayout();
    orderData = std::move(newData);
    indexes.clear();
    int i = 0;
    for (auto item: *orderData) {
        indexes[item.order_id] = i++;
    }
    emit endChangeLayout();
}

int OrderList::getItemStatus(int id) const {
    return getItem(id).status;
}

OrderData::const_iterator OrderList::begin() const {
    return orderData->begin();
}

OrderData::const_iterator OrderList::end() const {
    return orderData->end();
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