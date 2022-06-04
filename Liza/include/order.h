#pragma once

#include "cart.h"
#include "fwd.h"
#include "user.h"

using restbes::connectGet;

namespace restbesOrder {

std::string get_order_client_id(const std::string &order_id);

void update_order_history(id_t order_id, id_t client_id);

int get_order_timestamp(const std::string &order_id);

int get_order_cost(const std::string &order_id);

int get_order_status(const std::string &order_id);

std::string get_order_address(const std::string &order_id);

std::string get_order_comment(const std::string &order_id);

int get_order_last_modified(const std::string &order_id);

std::string get_order_items(const std::string &order_id);

class Order {
private:
    id_t m_order_id;
    id_t m_client_id;
    std::string m_address;
    std::string m_comment;
    restbes::OrderStatus m_order_status = restbes::CREATED;

public:
    explicit Order(id_t order_id) : m_order_id(order_id) {
        m_client_id =
            std::stoi(get_order_client_id(std::to_string(m_order_id)));
    };

    explicit Order(id_t client_id, std::string address, std::string comment)
        : m_client_id(client_id),
          m_address(std::move(address)),
          m_comment(std::move(comment)) {
        std::string cart = restbesCart::get_cart(std::to_string(m_client_id));

        int cost = restbesCart::cart_cost(cart);

        m_order_id = std::stoi(connectGet(
            R"(INSERT INTO "ORDER" ("ITEMS", "COST", "STATUS", "TIMESTAMP", "LAST_MODIFIED", "ADDRESS", "COMMENT") VALUES (')" +
            cart + "', " + std::to_string(cost) + ", " +
            std::to_string(m_order_status) + ", " +
            std::to_string(restbes::time_now) + ", " +
            std::to_string(restbes::time_now) + ", '" + m_address + "', '" +
            m_comment + "') RETURNING \"ORDER_ID\""));
    }

    [[nodiscard]] id_t get_order_id() const;
};

}  // namespace restbesOrder
