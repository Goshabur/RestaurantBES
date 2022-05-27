#include "CartList.h"

namespace restbes {

CartList::CartList(QObject *parent) : QObject(parent) {
}

int CartList::size() const {
    return items.rlock()->size();
}

CartItem CartList::getItemAt(int index) const {
    return items.rlock()->at(index);
}

void CartList::updateIndexes() {
    auto lockedItems = items.rlock();
    auto lockedIndexes = indexes.wlock();
    lockedIndexes->clear();
    int index = 0;
    for (const auto &elm : *lockedItems) {
        lockedIndexes->insert({elm.item_id, index++});
    }
}

void CartList::clearCart() {
    emit beginChangeLayout();
    items.wlock()->clear();
    indexes.wlock()->clear();
    emit indexesChanged();
    emit endChangeLayout();
}

bool CartList::setItemCount(int id, int value) {
    if (indexes.rlock()->count(id)) {
        int index = getIndex(id);
        auto item_count = items.rlock()->at(index).count;
        if (item_count == value) return false;
        if (value > 0) {
            items.wlock()->at(index).count = value;
            goto SUCCESS;
        } else if (value == 0) {
            emit beginRemoveItem(index);
            auto it = std::next(items.rlock()->begin(), index);
            items.wlock()->erase(it);
            updateIndexes();
            emit endRemoveItem();
            goto SUCCESS;
        }
    } else if (value > 0) {
        emit beginInsertItem(size());
        indexes.wlock()->insert({id, size()});
        items.wlock()->push_back({id, value});
        emit endInsertItem();
        goto SUCCESS;
    }
    return false;

    SUCCESS:
    emit itemCountChanged(id);
    return true;
}

int CartList::getItemCount(int id) const {
    if (indexes.rlock()->count(id) == 0) return 0;
    return items.rlock()->at(getIndex(id)).count;
}

int CartList::getIndex(int id) const {
    if (indexes.rlock()->count(id) == 0) return -1;
    return indexes.rlock()->at(id);
}

int CartList::getId(int index) const {
    return items.rlock()->at(index).item_id;
}

void CartList::setCart(std::shared_ptr<CartData> newData) {
    emit beginChangeLayout();
    *items.wlock() = std::move(*newData);
    updateIndexes();
    emit indexesChanged();
    emit endChangeLayout();
}


}