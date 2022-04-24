#pragma once

#include <iostream>
#include <map>
#include "fwd.h"
#include "nlohmann/json.hpp"

namespace restbes {

class Cart {
private:
    std::map<std::string, int> cart = {};

public:
    id_t client_id;
    Cart() : client_id(0){};

    [[nodiscard]] std::map<std::string, int> get_cart() const {
        std::string get_cart = R"(SELECT "CART" FROM "CART" WHERE "CLIENT_ID" = )" +
                               std::to_string(client_id);
        try {
            std::string j = restbes::connect_to_db_get(get_cart);

            auto json_parsed = nlohmann::json::parse(j);
            for (auto &el : json_parsed.items()) {
                std::cout << el.key() << ", quantity: " << el.value()
                          << " pcs\n";
            }
        } catch (std::exception const &e) {
            restbes::server_request_log << request << get_cart;
            restbes::server_error_log << error << e.what();
        }

        std::cout << "Total sum: " << total_price() << '\n';

        return cart;  // TODO: not needed if the client is logged in (?)
    }

    void add(id_t dish_id) const noexcept {
        //        ++cart[dish_id];

        try {
            int quantity = std::stoi(restbes::connect_to_db_get(
                R"(SELECT "CART" -> ')" + std::to_string(dish_id) +
                R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id)));

            nlohmann::json j;
            j[dish_id] = ++quantity;
            std::string final = restbes::connect_to_db_get(
                R"(SELECT "CART" || ')" + j.dump() +
                R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id));

            restbes::connect_to_db_exec(R"(UPDATE "CART" SET "CART" = ')" +
                                        final + R"(' WHERE "CLIENT_ID" = )" +
                                        std::to_string(client_id));

        } catch (...) {
            nlohmann::json j;
            j[dish_id] = 1;
            std::string final = restbes::connect_to_db_get(
                R"(SELECT "CART" || ')" + j.dump() +
                R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id));

            restbes::connect_to_db_exec(R"(UPDATE "CART" SET "CART" = ')" +
                                        final + R"(' WHERE "CLIENT_ID" = )" +
                                        std::to_string(client_id));
        }
    }

    void del(id_t dish_id) const noexcept {
        std::string delete_from_cart = "delete_from_cart";
        //        --cart[dish_id];

        //        if (cart[dish_id] <= 0) {
        //            cart.erase(dish_id);
        //        }

        try {
            int quantity = std::stoi(restbes::connect_to_db_get(
                R"(SELECT "CART" -> ')" + std::to_string(dish_id) +
                R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id)));

            nlohmann::json j;
            j[dish_id] = --quantity;
            std::string final;

            if (quantity <= 0) {
                final = restbes::connect_to_db_get(
                    R"(SELECT "CART" - ')" + std::to_string(dish_id) +
                    R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                    std::to_string(client_id));

            } else {
                final = restbes::connect_to_db_get(
                    R"(SELECT "CART" || ')" + j.dump() +
                    R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                    std::to_string(client_id));
            }

            restbes::connect_to_db_exec(R"(UPDATE "CART" SET "CART" = ')" +
                                        final + R"(' WHERE "CLIENT_ID" = )" +
                                        std::to_string(client_id));

            int cost = std::stoi(restbes::connect_to_db_get(
                R"(SELECT "COST" FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id)));

            cost -= std::stoi(restbes::connect_to_db_get(
                R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_NAME" = ')" +
                std::to_string(dish_id) + "'"));

            restbes::connect_to_db_exec(
                R"(UPDATE "CART" SET "COST" = )" + std::to_string(cost) +
                R"( WHERE "CLIENT_ID" = )" + std::to_string(client_id));

        } catch (std::exception const &e) {
            restbes::server_request_log << request << delete_from_cart;
            restbes::server_error_log << error << e.what();
        }
    }

    void clear() const noexcept {
        //        cart.clear();

        restbes::connect_to_db_exec(
            R"(UPDATE "CART" SET "CART" = '{}' WHERE "CLIENT_ID" = )" +
            std::to_string(client_id));

        restbes::connect_to_db_exec(
            R"(UPDATE "CART" SET "COST" = 0 WHERE "CLIENT_ID" = )" +
            std::to_string(client_id));
    }

    [[nodiscard]] int total_price() const {
        int cost = 0;

        std::string j = restbes::connect_to_db_get(
            R"(SELECT "CART" FROM "CART" WHERE "CLIENT_ID" = )" +
            std::to_string(client_id));

        auto json_parsed = nlohmann::json::parse(j);
        for (auto &el : json_parsed.items()) {
            cost += std::stoi(restbes::connect_to_db_get(
                        R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_NAME" = ')" +
                        el.key() + "'")) *
                    std::stoi(el.value().dump());
        }

        return cost;
    }
};

}  // namespace restbes
