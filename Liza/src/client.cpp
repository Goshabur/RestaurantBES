#include "../include/client.h"
#include "../include/fwd.h"
#include "../include/order.h"

namespace restbes {

std::string Client::get_client_id() const {
    return std::to_string(m_id);
}

std::string Client::get_client_name(const std::string& client_id) {
    return connectGet(R"(SELECT "NAME" FROM "CLIENT" WHERE "CLIENT_ID" = )" +
                      client_id);
}

std::string Client::get_client_email(const std::string& client_id) {
    return connectGet(R"(SELECT "EMAIL" FROM "CLIENT" WHERE "CLIENT_ID" = )" +
                      client_id);
}

std::string Client::get_client_cart(const std::string& client_id) {
    return connectGet(R"(SELECT "CART"::TEXT FROM "CLIENT" WHERE "CLIENT_ID" = )" +
                      client_id);
}

void update_order_history(id_t order_id, id_t client_id) {
    restbes::connectExec(
        R"(INSERT INTO "HISTORY" ("ORDER_ID", "CLIENT_ID") VALUES ()" +
        std::to_string(order_id) + ", " + std::to_string(client_id) +
        ")");
}

std::string Client::create_order(const std::string& address, const std::string& comment) const {
    restbes::Order order(m_id, address, comment);
    restbes::Cart::set_cart(std::to_string(m_id), "{}", 0);
    update_order_history(order.get_order_id(), m_id);

    return std::to_string(order.get_order_id());
}

std::string Client::cart() const {
    return restbes::Cart::get_cart(std::to_string(m_id));
}

}  // namespace restbes
