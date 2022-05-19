#include "MenuList.h"
#include "MenuModel.h"
#include "MenuData.h"
#include "CartList.h"
#include <QAbstractItemModel>

namespace restbes {

MenuList::MenuList(QObject *parent) : QObject(parent) {
}

// O(n)!
int MenuList::getId(int index) const {
    return *std::next(items.begin(), index);
}

int MenuList::getIndex(int id) const {
    if (items.count(id) == 0) return -1;
    int index = 0;
    for (auto i = items.begin(); *i < id; ++i) {
        ++index;
    }
    return index;
}

std::shared_ptr<MenuData> MenuList::getMenuData() const {
    return menuData;
}

std::shared_ptr<CartList> MenuList::getCartList() const {
    return cartList;
}

int MenuList::size() const {
    return menuData->data.size();
}

// O(n)!
bool MenuList::setItemAt(int index, const MenuItem &item) {
    if (index < 0 || index >= items.size())
        return false;

    int id = getId(index);
    MenuItem oldItem = menuData->getItem(id);
    if (item.id != id || item == oldItem) return false;

    if (menuData->setItem(id, item)) {
        emit itemChanged(id);
        return true;
    } else return false;
}

// O(n)!
MenuItem MenuList::getItemAt(int index) {
    if (index < 0 || index >= items.size())
        throw std::runtime_error("MenuList::getCartItemAt: index out of range");
    return menuData->getItem(getId(index));
}

void MenuList::setMenu(std::shared_ptr<MenuData> newData) {
    emit beginChangeLayout();
    items.clear();
    menuData = std::move(newData);
    for (const auto &it: menuData->data) {
        items.insert(it.first);
    }
    QModelIndexList newIndexList;
    newIndexList.reserve(size());
    if (parent()) {
        auto *parentModel = qobject_cast<MenuModel *>(parent());
        int index = 0;
        for (auto elm = items.begin(); elm != items.end(); ++elm, ++index) {
            newIndexList.push_back(parentModel->index(index, 0));
        }
        parentModel->updatePersistentIndexList(newIndexList);
    }
    emit endChangeLayout();
}

void MenuList::setCart(std::shared_ptr<CartList> newData) {
    cartList = std::move(newData);
    if (parent()) {
        auto *p = qobject_cast<MenuModel *>(parent());
        p->emit dataChanged(p->index(0, 0),
                            p->index(p->rowCount() - 1, 0),
                            QVector<int>() << p->CountRole);
    }
}

bool MenuList::setItemCount(int id, int value) {
    return cartList->setItemCount(id, value);
}

int MenuList::getItemCount(int id) const {
    return cartList->getItemCount(id);
}


// O(n)!
void MenuList::insertItem(MenuItem item) {
    if (menuData->data.count(item.id) > 0)
        throw std::runtime_error(
                "MenuList::insertItem: item with this id already exists");
    int index = 0;
    for (auto i = items.begin(); i != items.end() && *i < item.id; ++i) {
        ++index;
    }

    emit beginInsertItem(index);
    menuData->insertItem(item);
    items.insert(item.id);
    emit endInsertItem();
}

// O(n)!
void MenuList::removeItem(int id) {
    if (menuData->data.count(id) == 0)
        return;
    int index = 0;
    auto i = items.begin();
    for (; *i < id; ++i) {
        ++index;
    }

    emit beginInsertItem(index);
    menuData->removeItem(id);
    items.erase(i);
    emit endInsertItem();
}

void MenuList::removeUnavailableItems() {
    int index = 0;
    for (auto i = items.begin(); i != items.end(); ++index) {
        if (menuData->getItem(*i).status == 0) {
            emit beginRemoveItem(index);
            menuData->removeItem(*i);
            i = items.erase(i);
            emit endRemoveItem();
        } else {
            ++i;
        }
    }
}

}
