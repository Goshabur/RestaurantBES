#pragma once

#include <vector>

namespace restbes {

struct CartItem {
    int item_id = -1;
    int count = 1;

    friend bool operator==(const CartItem &a, const CartItem &b) {
        return a.item_id == b.item_id && a.count == b.count;
    }
};

using CartData = std::vector<CartItem>;

class CartList;

}