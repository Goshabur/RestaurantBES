#include "../include/admin.h"
#include "../include/fwd.h"
#include "../include/order.h"

namespace restbes {
void Admin::change_order_status(const std::string &order_id,
                                const std::string &set_status) {
    connectExec(R"(UPDATE "ORDER" SET "STATUS" = )" + set_status +
                " WHERE \"ORDER_ID\"=" + order_id);
    connectExec(R"(UPDATE "ORDER" SET "LAST_MODIFIED" = ')" +
                std::to_string(time_now) + "' WHERE \"ORDER_ID\"=" + order_id);

    restbes::Order::notifySessionsOrderChanged(order_id);
}

void Admin::change_dish_status(const std::string &dish_id,
                               const std::string &set_status) {
    connectExec(R"(UPDATE "DISH" SET "STATUS" = )" + set_status +
                " WHERE \"DISH_ID\"=" + dish_id);
    connectExec(R"(UPDATE "MENU_HISTORY" SET "TIMESTAMP" = ')" +
                std::to_string(time_now) + "'");

    restbes::notifySessionsMenuChanged();
}

void Admin::change_dish_price(const std::string &dish_id,
                              const std::string &set_price) {
    connectExec(R"(UPDATE "DISH" SET "PRICE" = )" + set_price +
                " WHERE \"DISH_ID\"= '" + dish_id + "'");
    connectExec(R"(UPDATE "MENU_HISTORY" SET "TIMESTAMP" = ')" +
                std::to_string(time_now) + "'");

    restbes::notifySessionsMenuChanged();
}

void Admin::change_dish_info(const std::string &dish_id,
                             const std::string &set_info) {
    connectExec(R"(UPDATE "DISH" SET "INFO" = ')" + set_info +
                "' WHERE \"DISH_ID\"= '" + dish_id + "'");
    connectExec(R"(UPDATE "MENU_HISTORY" SET "TIMESTAMP" = ')" +
                std::to_string(time_now) + "'");

    restbes::notifySessionsMenuChanged();
}

std::string Admin::getPrice(const std::string &dish_id) {
    return connectGet(R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_ID" = ')" +
                      dish_id + "'");
}

int Admin::getOrderStatus(const std::string &order_id) {
    return std::stoi(
        connectGet(R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = ')" +
                   order_id + "'"));
}

int Admin::getDishStatus(const std::string &dish_id) {
    return std::stoi(connectGet(
        R"(SELECT "STATUS" FROM "DISH" WHERE "DISH_ID" = ')" + dish_id + "'"));
}

std::string Admin::getDishName(const std::string &dish_id) {
    return connectGet(R"(SELECT "DISH_NAME" FROM "DISH" WHERE "DISH_ID" = ')" +
                      dish_id + "'");
}
}  // namespace restbes