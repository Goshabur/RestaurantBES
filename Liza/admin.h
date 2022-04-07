#pragma once

#include "client.h"

namespace restbes {

class Admin {
public:
    Admin() = default;

    static void change_order_status(id_t order_id, int set_status) noexcept;

    static void cancel_order(id_t order_id) noexcept;

    static void accept_order(id_t order_id) noexcept;

    static void change_dish_status(const std::string& dish_name, int set_status);

    static void change_dish_price(const std::string& dish_name, int set_price);

    static void change_dish_info(const std::string& dish_name, const std::string &set_info);
};
}  // namespace restbes
