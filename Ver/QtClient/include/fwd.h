#pragma once

#include <unordered_map>

namespace restbes {
struct CartItem; // item_id, count
class CartList;

// TODO:
//  class Order;
//  class OrderList;

struct MenuItem;
using MenuData = std::unordered_map<int, MenuItem>;
class MenuList;
class MenuModel; // MenuList* + CartList*

// TODO:
class Client; // CartList*

// TODO:
//  class jsonParser;
}
