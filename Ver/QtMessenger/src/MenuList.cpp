#include "MenuList.h"
#include <QAbstractItemModel>

namespace restbes {

MenuList::MenuList(QObject *parent) : QObject(parent) {
    insertItem({1, "Lasagna", "", 100, "Description", 1});
    insertItem({2, "Steak", "", 200, "Description", 1});
}

std::map<int, MenuItem> MenuList::items() const {
    return mItems;
}

int MenuList::size() const {
    return mItems.size();
}

bool MenuList::setItemAt(int index, const MenuItem &item) {
    if (index < 0 || index >= mItems.size())
        return false;

    const MenuItem &oldItem = getItemAt(index);
    if (item == oldItem)
        return false;

    findItemAt(index)->second = item;
    return true;
}

// O(n)!
std::map<int, MenuItem>::iterator MenuList::findItemAt(int index) {
    return std::next(mItems.begin(), index);
}

MenuItem MenuList::getItemAt(int index) {
    return findItemAt(index)->second;
}

// O(n)!
void MenuList::insertItem(MenuItem item) {
    int index = 0;
    for (auto i = mItems.begin();
         i != mItems.end() && i->first < item.id; ++i) {
        ++index;
    }

    emit beginInsertItem(index);
    mItems.insert({item.id, std::move(item)});
    emit endInsertItem();
}

void MenuList::removeUnavailableItems() {
    int index = 0;
    for (auto i = mItems.begin(); i != mItems.end(); ++index) {
        if (i->second.status == 0) {
            emit beginRemoveItem(index);
            i = mItems.erase(i);
            emit endRemoveItem();
        } else {
            ++i;
        }
    }
}

}
