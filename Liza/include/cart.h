#pragma once

#include "../../../folly_installed/installed/folly/include/folly/dynamic.h"
#include "../../../folly_installed/installed/folly/include/folly/json.h"
#include "fwd.h"
#include "nlohmann/json.hpp"

using folly::dynamic;
using folly::parseJson;
using nlohmann::json;
using restbes::connectExec;
using restbes::connectGet;

namespace restbes {

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

    static std::string get_cart(const std::string &user_id) {
        return connectGet(
            R"(SELECT "CART"::TEXT FROM "CART" WHERE "CLIENT_ID" = )" +
            user_id);
    }

    static void set_cart(const std::string &client_id,
                         const std::string &cart,
                         int cart_cost) {
        connectExec(R"(UPDATE "CART" SET "CART" = ')" + cart +
                    R"(' WHERE "CLIENT_ID" = )" + client_id);
        connectExec(R"(UPDATE "CART" SET "COST" = ')" +
                    std::to_string(cart_cost) + R"(' WHERE "CLIENT_ID" = )" +
                    client_id);
    }

    static void set_item_count(const std::string &client_id,
                               int dish_id,
                               int count) {
        dynamic new_cart = dynamic::array;

        auto cart = json::parse(restbes::Cart::get_cart(client_id));
        for (auto &el : cart) {
            dynamic item =
                dynamic::object("dish_id", el.at("dish_id").get<int>())(
                    "count", el.at("count").get<int>());

            if (el.at("dish_id").get<int>() == dish_id) {
                item["count"] = count;

                if (count == 0) {
                    continue;
                }
            }

            new_cart.push_back(item);
        }

        set_cart(client_id, folly::toJson(new_cart),
                 cart_cost(folly::toJson(new_cart)));
    }

    [[nodiscard]] static int cart_cost(const std::string &user_cart) {
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
};

}  // namespace restbes
