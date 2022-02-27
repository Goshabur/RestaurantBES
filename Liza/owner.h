#ifndef RESTAURANTBES_OWNER_H
#define RESTAURANTBES_OWNER_H

#include <string>

namespace owner {
    class Owner {
    public:
        Owner() = default;

        void change_dish_status(int dish_id, int set_status);

        void change_dish_price(int dish_id, int set_price);

        void change_dish_info(int dish_id, const std::string &set_info);
    };
}

#endif //RESTAURANTBES_OWNER_H
