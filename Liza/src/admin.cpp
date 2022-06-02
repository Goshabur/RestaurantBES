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

    restbes::Order order(std::stoi(order_id));
    order.notifySessionsOrderChanged();
}

void Admin::change_dish_status(const std::string &dish_id,
                               const std::string &set_status) {
    connectExec(R"(UPDATE "DISH" SET "STATUS" = )" + set_status +
                " WHERE \"DISH_ID\"=" + dish_id);
    connectExec(R"(UPDATE "MENU_HISTORY" SET "TIMESTAMP" = ')" +
                std::to_string(time_now) + "'");

    restbes::notifySessionsMenuChanged();
}

std::string Admin::add_new_dish(const std::string &dish_name,
                                const std::string &dish_price,
                                const std::string &dish_info,
                                const std::string &image_url) {
    std::string dish_id = connectGet(
        R"(INSERT INTO "DISH" ("DISH_NAME", "PRICE", "INFO", "IMAGE", "STATUS") VALUES (')" +
        dish_name + "', " + dish_price + ", '" + dish_info + "', '" +
        image_url + "', 1) RETURNING \"DISH_ID\"");

    restbes::notifySessionsMenuChanged();

    return dish_id;
}

void Admin::change_dish_price(const std::string &dish_id,
                              const std::string &set_price) {
    connectExec(R"(UPDATE "DISH" SET "PRICE" = )" + set_price +
                " WHERE \"DISH_ID\"= '" + dish_id + "'");
    connectExec(R"(UPDATE "MENU_HISTORY" SET "TIMESTAMP" = ')" +
                std::to_string(time_now) + "'");

    restbes::notifySessionsMenuChanged();
}

[[maybe_unused]] void Admin::change_dish_info(const std::string &dish_id,
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