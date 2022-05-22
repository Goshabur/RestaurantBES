#pragma once

#include <QString>

namespace restbes {

struct MenuItem {
    int id = -1;
    QString name = "DELETED";
    QString image = "";
    int price = 0;
    QString info = "This item doesn't exist";
    int status = 1;

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

}
