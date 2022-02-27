#ifndef RESTAURANTBES_CLIENT_H
#define RESTAURANTBES_CLIENT_H

#include <string>
#include <vector>
#include "server.h"

namespace client {
    static int global_id = 1;

    class Client {
    public:
        int id;
        std::string email;
        std::string name;
        std::vector<int> cart;
        int is_active_order = 0;  // = В данный момент у клиента отсутствует активный заказ (иначе -- id заказа)

        explicit Client(const std::string &name, const std::string &email) : id(global_id++), name(name), email(email) {
            db::connect_to_db_exec(
                    "INSERT INTO CLIENT (ID, EMAIL, NAME, ACTIVE_ORDER) VALUES (" + std::to_string(id) + ", " + email +
                    ", " + name + ", " + "0");
        };

        void get_dish_info(int dish_id);

        void add_dish_to_cart(int dish_id);

        void view_cart();

        [[nodiscard]] std::string get_order_status() const;

        void create_order() const;
    };
}

#endif //RESTAURANTBES_CLIENT_H
