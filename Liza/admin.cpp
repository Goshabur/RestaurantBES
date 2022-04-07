#include "admin.h"
#include "fwd.h"

void restbes::Admin::change_order_status(id_t order_id,
                                         int set_status) noexcept {
    restbes::connect_to_db_exec(R"(UPDATE "ORDER" set "STATUS" = )" +
                                std::to_string(set_status) +
                                " where \"ORDER_ID\"=" + std::to_string(order_id));
}

void restbes::Admin::cancel_order(id_t order_id) noexcept {
    change_order_status(order_id, 2);
}

void restbes::Admin::accept_order(id_t order_id) noexcept {
    change_order_status(order_id, 1);
}

void restbes::Admin::change_dish_status(const std::string& dish_name, int set_status) {
    restbes::connect_to_db_exec(R"(UPDATE "DISH" set "STATUS" = )" +
                                std::to_string(set_status) +
                                " where \"DISH_NAME\"=" + dish_name);
}

void restbes::Admin::change_dish_price(const std::string& dish_name, int set_price) {
    restbes::connect_to_db_exec(R"(UPDATE "DISH" set "PRICE" = )" +
                                std::to_string(set_price) +
                                " where \"DISH_NAME\"= '" + dish_name + "'");
}

void restbes::Admin::change_dish_info(const std::string& dish_name, const std::string &set_info) {
    restbes::connect_to_db_exec(R"(UPDATE "DISH" set "INFO" = ')" + set_info +
                                "' where \"DISH_NAME\"= '" + dish_name + "'");
}
