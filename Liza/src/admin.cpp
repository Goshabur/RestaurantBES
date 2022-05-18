#include "../include/admin.h"
#include "../include/fwd.h"

namespace restbes {
void Admin::change_order_status(const std::string &order_id,
                                const std::string &set_status) noexcept {
    connectExec(R"(UPDATE "ORDER" set "STATUS" = ')" + set_status +
                "' where \"ORDER_ID\"=" + order_id);
}

void Admin::change_dish_status(const std::string &dish_id, const std::string &set_status) {
    connectExec(R"(UPDATE "DISH" set "STATUS" = ')" + set_status +
                "' where \"DISH_ID\"=" + dish_id);
}

void Admin::change_dish_price(const std::string &dish_id,
                              const std::string &set_price) {
    connectExec(R"(UPDATE "DISH" set "PRICE" = )" + set_price +
                " where \"DISH_ID\"= '" + dish_id + "'");
}

void Admin::change_dish_info(const std::string &dish_id,
                             const std::string &set_info) {
    connectExec(R"(UPDATE "DISH" set "INFO" = ')" + set_info +
                "' where \"DISH_ID\"= '" + dish_id + "'");
}

std::string Admin::getPrice(const std::string &dish_id) {
    return connectGet(R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_ID" = ')" +
                      dish_id + "'");
}

std::string Admin::getOrderStatus(const std::string &order_id) {
    return connectGet(R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = ')" +
                      order_id + "'");
}

std::string Admin::getDishStatus(const std::string &dish_id) {
    return connectGet(R"(SELECT "STATUS" FROM "DISH" WHERE "DISH_ID" = ')" +
                      dish_id + "'");
}

std::string Admin::getDishName(const std::string &dish_id) {
    return connectGet(R"(SELECT "DISH_NAME" FROM "DISH" WHERE "DISH_ID" = ')" +
                      dish_id + "'");
}
}  // namespace restbes