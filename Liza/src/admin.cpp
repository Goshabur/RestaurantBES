#include "../include/admin.h"
#include "../include/fwd.h"
#include "../include/order.h"
#include "handlers.h"

using restbes::time_now;

namespace restbesAdmin {
bool check_admin(const std::string &password) {
    try {
        connectGet(
            R"(SELECT "ADMIN_ID" FROM "ADMINISTRATOR" WHERE "PASSWORD" = crypt(')" +
            password + "', \"PASSWORD\")");
    } catch (...) {
        restbes::server_request_log << "Failed attempt to log in as an administrator."
                           << std::endl;
        return false;
    }
    return true;
}

bool check_dish_exists(const std::string &dish_id) {
    try {
        connectGet(R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_ID" = ')" +
                   dish_id + "'");
    } catch (...) {
        return false;
    }
    return true;
}

bool check_order_exists(const std::string &order_id) {
    try {
        connectGet(R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = ')" +
                   order_id + "'");
    } catch (...) {
        return false;
    }
    return true;
}

void change_order_status(const std::string &order_id,
                         const std::string &set_status) {
    connectExec(R"(UPDATE "ORDER" SET "STATUS" = )" + set_status +
                " WHERE \"ORDER_ID\"=" + order_id);
    connectExec(R"(UPDATE "ORDER" SET "LAST_MODIFIED" = ')" +
                std::to_string(time_now) + "' WHERE \"ORDER_ID\"=" + order_id);

    restbesOrder::Order order(std::stoi(order_id));
    restbes::notifySessionsOrderChanged(order_id);
}

void change_dish_status(const std::string &dish_id,
                        const std::string &set_status) {
    connectExec(R"(UPDATE "DISH" SET "STATUS" = )" + set_status +
                " WHERE \"DISH_ID\"=" + dish_id);
    connectExec(R"(UPDATE "MENU_HISTORY" SET "TIMESTAMP" = ')" +
                std::to_string(time_now) + "'");

    restbes::notifySessionsMenuChanged();
}

std::string add_new_dish(const std::string &dish_name,
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

void change_dish_price(const std::string &dish_id,
                       const std::string &set_price) {
    connectExec(R"(UPDATE "DISH" SET "PRICE" = )" + set_price +
                " WHERE \"DISH_ID\"= '" + dish_id + "'");
    connectExec(R"(UPDATE "MENU_HISTORY" SET "TIMESTAMP" = ')" +
                std::to_string(time_now) + "'");

    restbes::notifySessionsMenuChanged();
}

void change_dish_info(const std::string &dish_id, const std::string &set_info) {
    connectExec(R"(UPDATE "DISH" SET "INFO" = ')" + set_info +
                "' WHERE \"DISH_ID\"= '" + dish_id + "'");
    connectExec(R"(UPDATE "MENU_HISTORY" SET "TIMESTAMP" = ')" +
                std::to_string(time_now) + "'");

    restbes::notifySessionsMenuChanged();
}

std::string getPrice(const std::string &dish_id) {
    return connectGet(R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_ID" = ')" +
                      dish_id + "'");
}

int getOrderStatus(const std::string &order_id) {
    return std::stoi(
        connectGet(R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = ')" +
                   order_id + "'"));
}

int getDishStatus(const std::string &dish_id) {
    return std::stoi(connectGet(
        R"(SELECT "STATUS" FROM "DISH" WHERE "DISH_ID" = ')" + dish_id + "'"));
}

std::string getDishName(const std::string &dish_id) {
    return connectGet(R"(SELECT "DISH_NAME" FROM "DISH" WHERE "DISH_ID" = ')" +
                      dish_id + "'");
}
}  // namespace restbes