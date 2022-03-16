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
        try {
            std::string j = restbes::connect_to_db_get(
                R"(SELECT "CART" FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id));

            auto json_parsed = nlohmann::json::parse(j);
            for (auto &el : json_parsed.items()) {
                std::cout << el.key() << ", quantity: " << el.value()
                          << " pcs\n";
            }
        } catch (...) {
        }

        std::cout << "Total sum: " << total_price() << '\n';

        return cart;  // TODO: not needed if the client is logged in (?)
    }

    void add(const std::string &dish_name) noexcept {
        ++cart[dish_name];

        try {
            int quantity = std::stoi(restbes::connect_to_db_get(
                R"(SELECT "CART" -> ')" + dish_name +
                R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id)));
            ++quantity;

            nlohmann::json j;
            j[dish_name] = quantity;
            std::string final = restbes::connect_to_db_get(
                R"(SELECT "CART" || ')" + j.dump() +
                R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id));

            restbes::connect_to_db_exec(R"(UPDATE "CART" SET "CART" = ')" +
                                        final + R"(' WHERE "CLIENT_ID" = )" +
                                        std::to_string(client_id));

        } catch (...) {
            nlohmann::json j;
            j[dish_name] = 1;
            std::string final = restbes::connect_to_db_get(
                R"(SELECT "CART" || ')" + j.dump() +
                R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id));

            restbes::connect_to_db_exec(R"(UPDATE "CART" SET "CART" = ')" +
                                        final + R"(' WHERE "CLIENT_ID" = )" +
                                        std::to_string(client_id));
        }

        int cost = std::stoi(restbes::connect_to_db_get(
            R"(SELECT "COST" FROM "CART" WHERE "CLIENT_ID" = )" +
            std::to_string(client_id)));

        cost += std::stoi(restbes::connect_to_db_get(
            R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_NAME" = ')" + dish_name +
            "'"));

        restbes::connect_to_db_exec(
            R"(UPDATE "CART" SET "COST" = )" + std::to_string(cost) +
            R"( WHERE "CLIENT_ID" = )" + std::to_string(client_id));
    }

    void del(const std::string &dish_name) noexcept {
        --cart[dish_name];

        if (cart[dish_name] <= 0) {
            cart.erase(dish_name);
        }

        try {
            int quantity = std::stoi(restbes::connect_to_db_get(
                R"(SELECT "CART" -> ')" + dish_name +
                R"(' FROM "CART" WHERE "CLIENT_ID" = )" +
                std::to_string(client_id)));
            --quantity;

            nlohmann::json j;
            j[dish_name] = quantity;
            std::string final;

            if (quantity <= 0) {
                final = restbes::connect_to_db_get(
                    R"(SELECT "CART" - ')" + dish_name +
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
                dish_name + "'"));

            restbes::connect_to_db_exec(
                R"(UPDATE "CART" SET "COST" = )" + std::to_string(cost) +
                R"( WHERE "CLIENT_ID" = )" + std::to_string(client_id));

        } catch (...) {
        }
    }

    void clear() noexcept {
        cart.clear();
        std::string final = "{}";

        restbes::connect_to_db_exec(
            R"(UPDATE "CART" SET "CART" = '{}' WHERE "CLIENT_ID" = )" +
            std::to_string(client_id));

        restbes::connect_to_db_exec(
            R"(UPDATE "CART" SET "COST" = 0 WHERE "CLIENT_ID" = )" +
            std::to_string(client_id));
    }

    [[nodiscard]] int total_price() const {
        int total = std::stoi(restbes::connect_to_db_get(
            R"(SELECT "COST" FROM "CART" WHERE "CLIENT_ID" = )" +
            std::to_string(client_id)));

        //            for (const auto &row: cart) {
        //                total += row.second * std::stoi(
        //                        restbes::connect_to_db_get(R"(SELECT "PRICE"
        //                        FROM "DISH" WHERE "DISH_NAME" = ')" +
        //                        row.first +
        //                                                   "'"));
        //            }  // TODO: needed if the client is not logged in
        //            (therefore no data is stored for them)

        return total;
    }
};

}  // namespace restbes
