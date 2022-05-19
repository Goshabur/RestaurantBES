#pragma once

#include <QString>

#include <unordered_map>

#include "fwd.h"

namespace restbes {

struct MenuItem {
    int id;
    QString name;
    QString image;
    int price;
    QString info;
    int status;

    friend bool operator!=(const MenuItem &a, const MenuItem &b) {
        return a.id != b.id ||
               a.price != b.price ||
               a.status != b.status ||
               a.info != b.info ||
               a.image != b.image ||
               a.name != b.name;
    }

    friend bool operator==(const MenuItem &a, const MenuItem &b) {
        return !(a != b);
    }
};

class MenuData {
private:
    std::unordered_map<int, MenuItem> data;

    [[nodiscard]] MenuItem getItem(int id) const;
    bool setItem(int id, const MenuItem &newItem);
    void insertItem(const MenuItem &item);
    void removeItem(int id);

    friend MenuList;
    friend CartList;
public:
    explicit MenuData(std::unordered_map<int, MenuItem> _data);
};

}