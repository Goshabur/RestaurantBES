#include "fwd.h"
#include "client.h"
#include "order.h"

namespace restbes {
    void Client::create_order() {
        restbes::Order y(cart);

        cart.clear();

        restbes::connect_to_db_exec(
                R"(UPDATE "CLIENT" set "ACTIVE_ORDER" = ')" + std::to_string(y.get_order_id()) + "' where \"ID\"=" +
                std::to_string(m_id));

        // TODO: signal to administrator so they could accept the order
    }

    std::string Client::get_active_order() const {
        return restbes::connect_to_db_get(
                R"(SELECT "ACTIVE_ORDER" FROM "CLIENT" WHERE "ID" = ')" + std::to_string(m_id) + "'");
    }

    std::string Client::get_order_status() const {
        return restbes::connect_to_db_get(
                R"(SELECT "STATUS" FROM "ORDER" WHERE "ID" = ')" + get_active_order() + "'");
    }

    void Client::add_to_cart(int dish_id) noexcept {
        cart.add(dish_id);
    }

    void Client::delete_from_cart(int dish_id) noexcept {
        cart.del(dish_id);
    }
}  // namespace restbes
