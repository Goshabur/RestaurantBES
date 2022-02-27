#include "owner.h"
#include "server.h"

namespace owner {
    void Owner::change_dish_status(int dish_id, int set_status) {
        db::connect_to_db_exec(
                "UPDATE DISH set STATUS = " + std::to_string(set_status) + " where ID=" + std::to_string(dish_id));
    }

    void Owner::change_dish_price(int dish_id, int set_price) {
        db::connect_to_db_exec(
                "UPDATE DISH set PRICE = " + std::to_string(set_price) + " where ID=" + std::to_string(dish_id));
    }

    void Owner::change_dish_info(int dish_id, const std::string &set_info) {
        db::connect_to_db_exec("UPDATE DISH set INFO = " + set_info + " where ID=" + std::to_string(dish_id));
    }

}
