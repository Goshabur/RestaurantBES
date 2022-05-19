#include "CartList.h"
#include "CartModel.h"
#include "MenuList.h"
#include <QDebug>

namespace restbes {

CartList::CartList(QObject *parent) : QObject(parent) {
}

void CartList::setMenuList(std::shared_ptr<MenuList> data) {
    menuList = std::move(data);
    menuData = menuList->getMenuData();
}

int CartList::size() const {
    return items.rlock()->size();
}

CartItem CartList::getCartItemAt(int index) const {
    return items.rlock()->at(index);
}

MenuItem CartList::getMenuItemAt(int index) const {
    return menuData->getItem(getId(index));
}

std::shared_ptr<MenuList> CartList::getMenuList() const {
    return menuList;
}

std::shared_ptr<MenuData> CartList::getMenuData() const {
    return menuData;
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
    auto *p = qobject_cast<CartModel *>(parent());
    if (p) p->updatePersistentIndexList({});
    emit endChangeLayout();
}

bool CartList::setItemCount(int id, int value) {
    if (indexes.rlock()->count(id)) {
        int index = getIndex(id);
        auto item_count = items.rlock()->at(index).count;
        if (item_count == value) return false;
        if (value > 0) {
            qDebug() << "Changing count of item --> id:" << id << " index:" << index;
            items.wlock()->at(index).count = value;
            goto SUCCESS;
        } else if (value == 0) {
            emit beginRemoveItem(index);
            qDebug() << "Removing item --> id:" << id << " index:" << index;
            auto it = std::next(items.rlock()->begin(), index);
            items.wlock()->erase(it);
            updateIndexes();
            emit endRemoveItem();
            goto SUCCESS;
        }
    } else if (value > 0) {
        emit beginInsertItem(size());
        qDebug() << "Inserting item --> id:" << id << " index:" << size();
        indexes.wlock()->insert({id, size()});
        items.wlock()->push_back({id, value});
        emit endInsertItem();
        goto SUCCESS;
    }
    return false;

    SUCCESS:
    qDebug() << "SUCCESS";
    auto lockedItems = items.rlock();
    int index = 0;
    for (auto ind : *lockedItems) {
        qDebug() << "index: " << index++ << " id:" << ind.item_id << " count:" << ind.count;
    }
    emit itemCountChanged(id);
    return true;
}

int CartList::getItemCount(int id) const {
    if (indexes.rlock()->count(id) == 0) return 0;
    return items.rlock()->at(getIndex(id)).count;
}

int CartList::getIndex(int id) const {
    return indexes.rlock()->at(id);
}

int CartList::getId(int index) const {
    return items.rlock()->at(index).item_id;
}


}