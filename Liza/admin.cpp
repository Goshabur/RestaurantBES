#include "admin.h"
#include "fwd.h"

void restbes::Admin::change_order_status(int order_id, int set_status) noexcept {
    restbes::connect_to_db_exec(
            R"(UPDATE "ORDER" set "STATUS" = )" + std::to_string(set_status) + " where \"ID\"=" +
            std::to_string(order_id));

}

void restbes::Admin::cancel_order(int order_id) noexcept {
    change_order_status(order_id, 2);
}

void restbes::Admin::accept_order(restbes::Client &user) noexcept {
    change_order_status(std::stoi(user.get_active_order()), 1);
}
