#include "../include/client.h"
#include "../include/fwd.h"
#include "../include/order.h"

namespace restbes {

std::string Client::get_client_id() const {
    return std::to_string(m_id);
}

std::string Client::create_order() {
    restbes::Order order(m_id);
    m_cart.clear();
    restbes::connectExec(
        R"(INSERT INTO "HISTORY" ("ORDER_ID", "CLIENT_ID") VALUES ()" +
        std::to_string(order.get_order_id()) + ", " + std::to_string(m_id) +
        ")");
    return std::to_string(order.get_order_id());
}

void Client::add_to_cart(id_t dish_id) const noexcept {
    m_cart.add(dish_id);
}

void Client::delete_from_cart(id_t dish_id) const noexcept {
    m_cart.del(dish_id);
}

void Client::empty_cart() const noexcept {
    m_cart.clear();
}

std::string Client::cart() const {
    return restbes::Cart::get_cart(m_id);
}

std::string Client::show_order_status(id_t order_id) {
    int status = std::stoi(
        connectGet(R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" +
                   std::to_string(order_id)));

    return orderStatuses[status];
}

}  // namespace restbes
