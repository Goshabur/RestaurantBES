#pragma once

#include <map>

namespace restbes {
    class Cart {
    private:
        std::map<int, int> cart = {};

    public:
        Cart() = default;

        std::map<int, int> get_cart() {
            return cart;
        }

        // shall it be private? all the methods
        void add(int dish_id) noexcept {
            ++cart[dish_id];
        }

        void del(int dish_id) noexcept {
            --cart[dish_id];

            if (cart[dish_id] <= 0) {
                cart.erase(dish_id);
            }
        }

        void clear() noexcept {
            cart.clear();
        }
    };
}  // namespace restbes
