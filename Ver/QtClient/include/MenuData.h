#pragma once

#include <QString>

#include <unordered_map>

#include "fwd.h"
#include "MenuItem.h"

namespace restbes {

class MenuData {
private:
    std::unordered_map<int, MenuItem> data;

    [[nodiscard]] MenuItem getItem(int id) const;

    [[nodiscard]] int size() const;

    bool setItem(int id, const MenuItem &newItem);

    void insertItem(const MenuItem &item);

    void removeItem(int id);

    friend MenuList;

public:
    explicit MenuData(std::unordered_map<int, MenuItem> _data);
};

}