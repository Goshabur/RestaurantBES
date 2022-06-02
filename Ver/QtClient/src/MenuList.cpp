#include "MenuList.h"
#include "MenuItem.h"

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

std::set<int>::const_iterator MenuList::begin() const {
    return items.begin();
}

std::set<int>::const_iterator MenuList::end() const {
    return items.end();
}

std::shared_ptr<MenuData> MenuList::getMenuData() const {
    return menuData;
}

int MenuList::size() const {
    return menuData->size();
}

// O(n)!
bool MenuList::setItemAt(int index, const MenuItem &item) {
    if (index < 0 || index >= items.size())
        return false;

    int id = getId(index);
    if (menuData->count(id) == 0 || menuData->at(id) == item)
        return false;

    menuData->at(id) = item;
    emit itemChanged(id);
    return true;
}

// O(n)!
MenuItem MenuList::getItemAt(int index) const {
    if (index < 0 || index >= items.size())
        throw std::runtime_error("MenuList::getItemAt: index out of range");
    int id = getId(index);
    return getItem(id);
}

MenuItem MenuList::getItem(int id) const {
    return (menuData->count(id)) ? menuData->at(id) : MenuItem{};
}

void MenuList::setMenu(std::shared_ptr<MenuData> newData) {
    emit beginChangeLayout();
    items.clear();
    menuData = std::move(newData);
    for (const auto &it: *menuData) {
        items.insert(it.first);
    }
    emit endChangeLayout();
}

// O(n)!
void MenuList::insertItem(MenuItem item) {
    if (menuData->count(item.id) > 0)
        throw std::runtime_error(
                "MenuList::insertItem: item with this id already exists");
    int index = 0;
    for (auto i = items.begin(); i != items.end() && *i < item.id; ++i) {
        ++index;
    }

    emit beginInsertItem(index);
    menuData->insert({item.id, item});
    items.insert(item.id);
    emit endInsertItem();
}

// O(n)!
void MenuList::removeItem(int id) {
    if (menuData->count(id) == 0)
        return;
    int index = 0;
    auto i = items.begin();
    for (; *i < id; ++i) {
        ++index;
    }

    emit beginInsertItem(index);
    menuData->erase(id);
    items.erase(i);
    emit endInsertItem();
}

void MenuList::removeUnavailableItems() {
    int index = 0;
    for (auto i = items.begin(); i != items.end(); ++index) {
        if (getItem(*i).status == 0) {
            emit beginRemoveItem(index);
            removeItem(*i);
            i = items.erase(i);
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
