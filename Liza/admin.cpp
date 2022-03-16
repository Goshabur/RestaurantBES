#include "admin.h"
#include "fwd.h"

void restbes::Admin::change_order_status(id_t order_id,
                                         int set_status) noexcept {
    restbes::connect_to_db_exec(R"(UPDATE "ORDER" set "STATUS" = )" +
                                std::to_string(set_status) +
                                " where \"ID\"=" + std::to_string(order_id));
}

void restbes::Admin::cancel_order(id_t order_id) noexcept {
    change_order_status(order_id, 2);
}

void restbes::Admin::accept_order(id_t order_id) noexcept {
    change_order_status(order_id, 1);
}
