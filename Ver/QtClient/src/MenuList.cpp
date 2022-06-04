#include "MenuList.h"
#include "MenuItem.h"

#include <QAbstractItemModel>

namespace restbes {

MenuList::MenuList(QObject *parent) : QObject(parent) {
}

// O(n)!
int MenuList::getId(int index) const {
    return *std::next(begin(), index);
}

int MenuList::getIndex(int id) const {
    if (items.rlock()->count(id) == 0) return -1;
    int index = 0;
    for (auto i = begin(); *i < id; ++i) {
        ++index;
    }
    return index;
}

std::set<int>::const_iterator MenuList::begin() const {
    return items.rlock()->begin();
}

std::set<int>::const_iterator MenuList::end() const {
    return items.rlock()->end();
}

int MenuList::size() const {
    return menuData.rlock()->size();
}

// O(n)!
bool MenuList::setItemAt(int index, const MenuItem &item) {
    if (index < 0 || index >= size())
        return false;

    int id = getId(index);
    if (menuData.rlock()->count(id) == 0 || menuData.rlock()->at(id) == item)
        return false;

    menuData.wlock()->at(id) = item;
    emit itemChanged(id);
    return true;
}

// O(n)!
MenuItem MenuList::getItemAt(int index) const {
    if (index < 0 || index >= size())
        throw std::runtime_error("MenuList::getItemAt: index out of range");
    int id = getId(index);
    return getItem(id);
}

MenuItem MenuList::getItem(int id) const {
    auto lockedData = menuData.rlock();
    return (lockedData->count(id)) ? lockedData->at(id) : MenuItem{};
}

void MenuList::setMenu(MenuData newData) {
    emit beginChangeLayout();
    auto lockedItems = items.wlock();
    auto lockedData = menuData.wlock();
    lockedItems->clear();
    *lockedData = std::move(newData);
    for (const auto &it: *lockedData) {
        lockedItems->insert(it.first);
    }
    emit endChangeLayout();
}

// O(n)!
void MenuList::insertItem(const MenuItem& item) {
    if (menuData.rlock()->count(item.id) > 0)
        throw std::runtime_error(
                "MenuList::insertItem: item with this id already exists");
    int index = 0;
    for (auto i = begin(); i != end() && *i < item.id; ++i) {
        ++index;
    }

    emit beginInsertItem(index);
    menuData.wlock()->insert({item.id, item});
    items.wlock()->insert(item.id);
    emit endInsertItem();
}

// O(n)!
void MenuList::removeItem(int id) {
    if (menuData.rlock()->count(id) == 0)
        return;
    int index = 0;
    auto i = begin();
    for (; *i < id; ++i) {
        ++index;
    }

    emit beginInsertItem(index);
    menuData.wlock()->erase(id);
    items.wlock()->erase(i);
    emit endInsertItem();
}

void MenuList::removeUnavailableItems() {
    int index = 0;
    for (auto i = begin(); i != end(); ++index) {
        if (getItem(*i).status == 0) {
            emit beginRemoveItem(index);
            removeItem(*i);
            i = items.wlock()->erase(i);
            emit endRemoveItem();
        } else {
            ++i;
        }
    }
}

void MenuList::setTimestamp(unsigned int newTimestamp) {
    timestamp = newTimestamp;
}

unsigned int MenuList::getTimestamp() const {
    return timestamp;
}

}
