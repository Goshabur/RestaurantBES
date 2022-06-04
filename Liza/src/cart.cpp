#include "cart.h"

namespace restbesCart {

int cart_cost(const std::string &user_cart) {
    int cost = 0;

    auto cart = json::parse(user_cart);
    for (auto &el : cart) {
        cost += std::stoi(connectGet(
                    R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_ID" = ')" +
                    std::to_string(el.at("dish_id").get<int>()) + "'")) *
                el.at("count").get<int>();
    }

    return cost;
}

std::string get_cart(const std::string &user_id) {
    return connectGet(
        R"(SELECT "CART"::TEXT FROM "CART" WHERE "CLIENT_ID" = )" + user_id);
}

void set_cart(const std::string &client_id,
              const std::string &cart,
              int cart_cost) {
    connectExec(R"(UPDATE "CART" SET "CART" = ')" + cart +
                R"(' WHERE "CLIENT_ID" = )" + client_id);
    connectExec(R"(UPDATE "CART" SET "COST" = )" + std::to_string(cart_cost) +
                R"( WHERE "CLIENT_ID" = )" + client_id);
}

void set_item_count(const std::string &client_id, int dish_id, int count) {
    dynamic new_cart = dynamic::array;
    bool existsInCart = false;

    auto cart = json::parse(restbesCart::get_cart(client_id));
    for (auto &el : cart) {
        dynamic item = dynamic::object("dish_id", el.at("dish_id").get<int>())(
            "count", el.at("count").get<int>());

        if (el.at("dish_id").get<int>() == dish_id) {
            if (count == 0) {
                continue;
            }

            item["count"] = count;
            existsInCart = true;
        }
        new_cart.push_back(item);
    }

    if (!existsInCart && count != 0) {
        dynamic item = dynamic::object("dish_id", dish_id)(
            "count", count);
        new_cart.push_back(item);
    }

    set_cart(client_id, folly::toJson(new_cart),
             cart_cost(folly::toJson(new_cart)));
}
}  // namespace restbesCart