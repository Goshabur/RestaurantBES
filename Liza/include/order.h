#pragma once

#include "../include/cart.h"
#include "../include/fwd.h"

using restbes::connectGet;

namespace restbes {

class Order {
private:
    id_t m_order_id;
    id_t m_client_id;
    std::string m_address;
    std::string m_comment;
    OrderStatus m_order_status = CREATED;

public:
    explicit Order(id_t order_id) : m_order_id(order_id) {
        m_client_id =
            std::stoi(get_order_client_id(std::to_string(m_order_id)));
    };

    explicit Order(id_t client_id, std::string address, std::string comment)
        : m_client_id(client_id),
          m_address(std::move(address)),
          m_comment(std::move(comment)) {
        std::string cart = restbes::Cart::get_cart(std::to_string(m_client_id));

        int cost = restbes::Cart::cart_cost(cart);

        m_order_id = std::stoi(connectGet(
            R"(INSERT INTO "ORDER" ("ITEMS", "COST", "STATUS", "TIMESTAMP", "LAST_MODIFIED", "ADDRESS", "COMMENT") VALUES (')" +
            cart + "', " + std::to_string(cost) + ", " +
            std::to_string(m_order_status) + ", '" + std::to_string(time_now) +
            "', '" + std::to_string(time_now) + "', '" + m_address + "', " +
            m_comment + "') RETURNING \"ORDER_ID\""));
    }

    [[nodiscard]] id_t get_order_id() const noexcept {
        return m_order_id;
    }

    static std::string get_order_timestamp(const std::string &order_id) {
        return connectGet(
            R"(SELECT "TIMESTAMP" FROM "ORDER" WHERE "ORDER_ID" = )" +
            order_id);
    }

    static int get_order_cost(const std::string &order_id) {
        return std::stoi(connectGet(
            R"(SELECT "COST" FROM "ORDER" WHERE "ORDER_ID" = )" + order_id));
    }

    static int get_order_status(const std::string &order_id) {
        return std::stoi(connectGet(
            R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" + order_id));
    }

    static std::string get_order_address(const std::string &order_id) {
        return connectGet(
            R"(SELECT "ADDRESS" FROM "ORDER" WHERE "ORDER_ID" = )" + order_id);
    }

    static std::string get_order_client_id(const std::string &order_id) {
        return connectGet(
            R"(SELECT "CLIENT_ID" FROM "HISTORY" WHERE "ORDER_ID" = )" +
            order_id);
    }

    static std::string get_order_comment(const std::string &order_id) {
        return connectGet(
            R"(SELECT "COMMENT" FROM "ORDER" WHERE "ORDER_ID" = )" + order_id);
    }

    static std::string get_order_last_modified(const std::string &order_id) {
        return connectGet(
            R"(SELECT "LAST_MODIFIED" FROM "ORDER" WHERE "ORDER_ID" = )" +
            order_id);
    }

    static std::string get_order_items(const std::string &order_id) {
        return connectGet(
            R"(SELECT "ITEMS"::TEXT FROM "ORDER" WHERE "ORDER_ID" = )" +
            order_id);
    }

    void notifySessionsOrderChanged() const {
        folly::dynamic notificationJson = folly::dynamic::object;
        notificationJson["event"] = "order_changed";
        notificationJson["timestamp"] = connectGet(
            R"(SELECT "LAST_MODIFIED" FROM "ORDER" WHERE "ORDER_ID = )" +
            std::to_string(m_order_id));
        notificationJson["body"] = folly::dynamic::object;
        notificationJson["body"]["order_id"] = m_order_id;

        std::string user_id =
            restbes::Order::get_order_client_id(std::to_string(m_order_id));
        auto user = server->getUser(std::to_string(m_client_id));

        user->push(restbes::generateResponse(folly::toJson(notificationJson),
                                             "application/json",
                                             restbes::Connection::KEEP_ALIVE));
    }
};

}  // namespace restbes
