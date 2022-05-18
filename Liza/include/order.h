#pragma once

#include "fwd.h"
#include "nlohmann/json.hpp"

using restbes::connectGet;

namespace restbes {

class Order {
private:
    id_t m_order_id;
    id_t m_client_id;
    OrderStatus m_order_status = CREATED;

public:
    explicit Order(id_t client_id) : m_client_id(client_id) {
        std::string cart =
            connectGet(R"(SELECT "CART" FROM "CART" WHERE "CLIENT_ID" = )" +
                       std::to_string(m_client_id));

        std::string cost =
            connectGet(R"(SELECT "COST" FROM "CART" WHERE "CLIENT_ID" = )" +
                       std::to_string(m_client_id));

        m_order_id = std::stoi(connectGet(
            R"(INSERT INTO "ORDER" ("ITEMS", "COST", "STATUS", "TIME") VALUES (')" +
            cart + "', " + cost + ", " + orderStatuses[m_order_status] +
            ", NOW()) RETURNING \"ORDER_ID\""));
    }

    [[nodiscard]] id_t get_order_id() const noexcept {
        return m_order_id;
    }
};

}  // namespace restbes
