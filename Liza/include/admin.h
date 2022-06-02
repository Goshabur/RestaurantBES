#pragma once

#include <cstdlib>
#include "string"

namespace restbes {

class Admin {
public:
    Admin() = default;

    static std::string add_new_dish(const std::string &dish_name,
                             const std::string &dish_price,
                             const std::string &dish_info,
                             const std::string &image_url);

    static void change_order_status(const std::string &order_id,
                                    const std::string &set_status);

    static void change_dish_status(const std::string &dish_name,
                                   const std::string &set_status);

    static void change_dish_price(const std::string &dish_id,
                                  const std::string &set_price);

    static void change_dish_info(const std::string &dish_id,
                                 const std::string &set_info);

    static std::string getPrice(const std::string &dish_id);

    static int getDishStatus(const std::string &dish_id);

    static int getOrderStatus(const std::string &order_id);

    static std::string getDishName(const std::string &dish_id);
};
}  // namespace restbes
