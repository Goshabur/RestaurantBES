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

bool OrderList::setItemStatus(int id, int value) {
    if (indexes.count(id)) {
        auto status = getItemStatus(id);
        if (status == value) return false;
        orderData->at(getIndex(id)).status = value;
        goto SUCCESS;
    } else {
        emit beginInsertItem(size());
        indexes.insert({id, size()});
        orderData->push_back({id, value});
        emit endInsertItem();
        goto SUCCESS;
    }

    SUCCESS:
    emit itemChanged(id);
    return true;
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

uint32_t OrderList::getTimestamp() const {
    return timestamp;
}

void OrderList::setTimestamp(uint32_t newTimestamp) {
    timestamp = newTimestamp;
}

}