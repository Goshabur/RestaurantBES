#ifndef RESTAURANTBES_ADMIN_H
#define RESTAURANTBES_ADMIN_H

#include "client.h"
#include <string>

namespace admin {
    class Admin {
    public:
        Admin() = default;

        static void change_order_status(int order_id, int set_status);

        void cancel_order(int order_id);

        void accept_order(client::Client &user);
    };
}

#endif //RESTAURANTBES_ADMIN_H
