#include "owner.h"

namespace restbes {

void Owner::change_dish_status(id_t dish_id, int set_status) {
    restbes::connect_to_db_exec(R"(UPDATE "DISH" set "STATUS" = )" +
                                std::to_string(set_status) +
                                " where \"ID\"=" + std::to_string(dish_id));
}

void Owner::change_dish_price(id_t dish_id, int set_price) {
    restbes::connect_to_db_exec(R"(UPDATE "DISH" set "PRICE" = )" +
                                std::to_string(set_price) +
                                " where \"ID\"=" + std::to_string(dish_id));
}

void Owner::change_dish_info(id_t dish_id, const std::string &set_info) {
    restbes::connect_to_db_exec(R"(UPDATE "DISH" set "INFO" = ')" + set_info +
                                "' where \"ID\"=" + std::to_string(dish_id));
}

}  // namespace restbes
