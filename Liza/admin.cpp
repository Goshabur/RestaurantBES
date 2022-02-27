#include "admin.h"

void admin::Admin::change_order_status(int order_id, int set_status) {
    db::connect_to_db_exec(
            "UPDATE ORDER set STATUS = " + std::to_string(set_status) + " where ID=" + std::to_string(order_id));
}

void admin::Admin::cancel_order(int order_id) {
    change_order_status(order_id, 2);
}

void admin::Admin::accept_order(client::Client &user) {
    change_order_status(user.is_active_order, 1);
}
