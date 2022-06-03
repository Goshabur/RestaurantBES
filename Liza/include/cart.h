#pragma once

#include "../include/fwd.h"

using folly::dynamic;
using folly::parseJson;
using nlohmann::json;
using restbes::connectExec;
using restbes::connectGet;

namespace restbesCart {

int cart_cost(const std::string &user_cart);

std::string get_cart(const std::string &user_id);

void set_cart(const std::string &client_id,
              const std::string &cart,
              int cart_cost);

void set_item_count(const std::string &client_id, int dish_id, int count);

class Cart {
public:
    id_t client_id;

    Cart() = default;

    explicit Cart(id_t user_id, const std::string &cart) : client_id(user_id) {
        connectExec(
            R"(INSERT INTO "CART" ("CLIENT_ID", "COST", "CART") VALUES ()" +
            std::to_string(client_id) + ", " + std::to_string(cart_cost(cart)) +
            ", '" + cart + "')");
    }
};

}  // namespace restbesCart
