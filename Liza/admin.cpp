#include "admin.h"
#include "fwd.h"

void restbes::Admin::change_order_status(id_t order_id,
                                         OrderStatus set_status) noexcept {
    restbes::connectExec(R"(UPDATE "ORDER" set "STATUS" = )" +
                         Statuses[set_status] +
                         " where \"ORDER_ID\"=" + std::to_string(order_id));
}

void restbes::Admin::change_dish_status(const std::string &dish_name,
                                        int set_status) {
    restbes::connectExec(R"(UPDATE "DISH" set "STATUS" = )" +
                         std::to_string(set_status) +
                         " where \"DISH_NAME\"=" + dish_name);
}

void restbes::Admin::change_dish_price(const std::string &dish_name,
                                       int set_price) {
    restbes::connectExec(R"(UPDATE "DISH" set "PRICE" = )" +
                         std::to_string(set_price) + " where \"DISH_NAME\"= '" +
                         dish_name + "'");
}

void restbes::Admin::change_dish_info(const std::string &dish_name,
                                      const std::string &set_info) {
    restbes::connectExec(R"(UPDATE "DISH" set "INFO" = ')" + set_info +
                         "' where \"DISH_NAME\"= '" + dish_name + "'");
}
