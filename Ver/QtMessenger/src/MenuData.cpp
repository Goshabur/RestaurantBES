#include "MenuData.h"

#include <utility>

namespace restbes {

MenuData::MenuData(std::unordered_map<int, MenuItem> _data) : data(std::move(_data)) {

}

MenuItem MenuData::getItem(int id) const {
    return data.at(id);
}

bool MenuData::setItem(int id, const MenuItem &newItem) {
    if (data.count(id) == 0) return false;

    data.at(id) = newItem;
    return true;
}

void MenuData::insertItem(const MenuItem &item) {
    data.insert({item.id, item});
}

void MenuData::removeItem(int id) {
    data.erase(id);
}

}