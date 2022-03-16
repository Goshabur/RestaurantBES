#pragma once

#include "fwd.h"
#include "nlohmann/json.hpp"

namespace restbes {

class Order {
private:
    id_t m_order_id;
    id_t m_client_id;

    enum orderStatus {
        CREATED = 0,
        ACCEPTED = 1,
        CANCELLED = 2,
        IN_PROGRESS = 3,
        COMPLETED = 4
    };

    orderStatus m_order_status = CREATED;

public:
    explicit Order(id_t client_id) : m_client_id(client_id) {
        std::string cart = restbes::connect_to_db_get(
            R"(SELECT "ITEMS" FROM "CART" WHERE "CLIENT_ID" = )" +
            std::to_string(m_client_id));

        std::string cost = restbes::connect_to_db_get(
            R"(SELECT "COST" FROM "CART" WHERE "CLIENT_ID" = )" +
            std::to_string(m_client_id));

        m_order_id = std::stoi(restbes::connect_to_db_get(
            R"(INSERT INTO "ORDER" ("ITEMS", "COST", "STATUS", "TIME") VALUES ()" +
            cart + cost + std::to_string(m_order_status) +
            ", NOW()) RETURNING \"ORDER_ID\""));
    }

    [[nodiscard]] id_t get_order_id() const noexcept {
        return m_order_id;
    }
};

}  // namespace restbes
