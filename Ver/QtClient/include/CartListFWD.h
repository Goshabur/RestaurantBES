#pragma once

#include <vector>

namespace restbes {

struct CartItem {
    int item_id = -1;
    int count = 1;
};

using CartData = std::vector<CartItem>;

class CartList;

}