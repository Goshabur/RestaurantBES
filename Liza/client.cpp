#include "client.h"
#include "fwd.h"
#include "order.h"

namespace restbes {

void Client::create_order() {
    restbes::Order order(m_id);
    m_cart.clear();

    restbes::connect_to_db_exec(
        R"(INSERT INTO "ACTIVE_ORDER" ("ORDER_ID", "CLIENT_ID") VALUES ()" +
        std::to_string(order.get_order_id()) + ", " + std::to_string(m_id) +
        ")");

    // TODO: signal to administrator so they could accept the order
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

std::map<std::string, int> Client::cart() const {
    return m_cart.get_cart();
}

id_t Client::get_client_id() const noexcept {
    return m_id;
}

}  // namespace restbes
