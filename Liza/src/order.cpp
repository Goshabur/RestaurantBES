#include "order.h"

namespace restbesOrder {

std::string get_order_client_id(const std::string &order_id) {
    return connectGet(
        R"(SELECT "CLIENT_ID" FROM "HISTORY" WHERE "ORDER_ID" = )" + order_id);
}

void update_order_history(id_t order_id, id_t client_id) {
    restbes::connectExec(
        R"(INSERT INTO "HISTORY" ("ORDER_ID", "CLIENT_ID") VALUES ()" +
        std::to_string(order_id) + ", " + std::to_string(client_id) + ")");
}

int get_order_timestamp(const std::string &order_id) {
    return std::stoi(connectGet(
        R"(SELECT "TIMESTAMP" FROM "ORDER" WHERE "ORDER_ID" = )" + order_id));
}

int get_order_cost(const std::string &order_id) {
    return std::stoi(connectGet(
        R"(SELECT "COST" FROM "ORDER" WHERE "ORDER_ID" = )" + order_id));
}

int get_order_status(const std::string &order_id) {
    return std::stoi(connectGet(
        R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" + order_id));
}

std::string get_order_address(const std::string &order_id) {
    return connectGet(R"(SELECT "ADDRESS" FROM "ORDER" WHERE "ORDER_ID" = )" +
                      order_id);
}

std::string get_order_comment(const std::string &order_id) {
    return connectGet(R"(SELECT "COMMENT" FROM "ORDER" WHERE "ORDER_ID" = )" +
                      order_id);
}

int get_order_last_modified(const std::string &order_id) {
    return std::stoi(connectGet(
        R"(SELECT "LAST_MODIFIED" FROM "ORDER" WHERE "ORDER_ID" = )" +
        order_id));
}

std::string get_order_items(const std::string &order_id) {
    return connectGet(
        R"(SELECT "ITEMS"::TEXT FROM "ORDER" WHERE "ORDER_ID" = )" + order_id);
}

id_t Order::get_order_id() const {
    return m_order_id;
}

}  // namespace restbesOrder