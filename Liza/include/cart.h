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
            std::to_string(client_id) + ", " + total_price(cart) + ", '" +
            cart + "')");
    }

    [[nodiscard]] static std::string get_cart(id_t user_id) {
        std::string cart =
            connectGet(R"(SELECT "CART" FROM "CART" WHERE "CLIENT_ID" = )" +
                       std::to_string(user_id));

        dynamic responseJson =
            dynamic::object("cart", cart)("total_price", total_price(cart));

        auto responseStr = folly::toJson(responseJson);

        return responseStr;
    }

    void add(id_t dish_id) const noexcept {
        int quantity = 0;
        json addToCart;

        try {
            quantity = std::stoi(
                connectGet(R"(SELECT "CART" -> ')" + std::to_string(dish_id) +
                           R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                           std::to_string(client_id)));

        } catch (...) {
            // no such product in a cart
        }

        addToCart[dish_id] = ++quantity;
        std::string resultCart =
            connectGet(R"(SELECT "CART" || ')" + addToCart.dump() +
                       R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                       std::to_string(client_id));

        connectExec(R"(UPDATE "CART" SET "CART" = ')" + resultCart +
                    R"(' WHERE "CLIENT_ID" = )" + std::to_string(client_id));
    }

    void del(id_t dish_id) const noexcept {
        int quantity = std::stoi(
            connectGet(R"(SELECT "CART" -> ')" + std::to_string(dish_id) +
                       R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                       std::to_string(client_id)));

        json addToCart;
        addToCart[dish_id] = --quantity;
        std::string resultCart;

        if (quantity <= 0) {
            resultCart =
                connectGet(R"(SELECT "CART" - ')" + std::to_string(dish_id) +
                           R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                           std::to_string(client_id));
        } else {
            resultCart = connectGet(R"(SELECT "CART" || ')" + addToCart.dump() +
                                    R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                                    std::to_string(client_id));
        }

        connectExec(R"(UPDATE "CART" SET "CART" = ')" + resultCart +
                    R"(' WHERE "CLIENT_ID" = )" + std::to_string(client_id));

        connectExec(R"(UPDATE "CART" SET "COST" = )" + total_price(resultCart) +
                    R"( WHERE "CLIENT_ID" = )" + std::to_string(client_id));
    }

    void clear() const noexcept {
        connectExec(R"(UPDATE "CART" SET "CART" = '{}' WHERE "CLIENT_ID" = )" +
                    std::to_string(client_id));

        connectExec(R"(UPDATE "CART" SET "COST" = 0 WHERE "CLIENT_ID" = )" +
                    std::to_string(client_id));
    }

    [[nodiscard]] static std::string total_price(const std::string &user_cart) {
        int cost = 0;
        json cart(user_cart);

        for (auto &el : cart.items()) {
            cost += std::stoi(connectGet(
                        R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_NAME" = ')" +
                        el.key() + "'")) *
                    std::stoi(el.value().dump());
        }

        return std::to_string(cost);
    }
};

}  // namespace restbes
