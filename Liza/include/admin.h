#pragma once

#include <cstdlib>
#include "string"

namespace restbesAdmin {

bool check_admin(const std::string &password);

bool check_dish_exists(const std::string &dish_id);

bool check_order_exists(const std::string &order_id);

std::string add_new_dish(const std::string &dish_name,
                         const std::string &dish_price,
                         const std::string &image_url);

void change_order_status(const std::string &order_id,
                         const std::string &set_status);

void change_dish_status(const std::string &dish_name,
                        const std::string &set_status);

void change_dish_price(const std::string &dish_id,
                       const std::string &set_price);

void change_dish_info(const std::string &dish_id, const std::string &set_info);

std::string getPrice(const std::string &dish_id);

int getDishStatus(const std::string &dish_id);

int getOrderStatus(const std::string &order_id);

std::string getDishName(const std::string &dish_id);

}  // namespace restbesAdmin
