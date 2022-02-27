#ifndef RESTAURANTBES_ORDER_H
#define RESTAURANTBES_ORDER_H

#include "server.h"

namespace order {
    static int global_id = 1;

    class Order {
    public:
        explicit Order(const std::vector<int> &cart) : id(global_id++) {
            db::connect_to_db_exec("INSERT INTO ORDER (ID, STATUS) VALUES (" +
                                   std::to_string(id) + ", " +
                                   std::to_string(status)); //+
//            ", filling...");  // TODO

//            for (int item : cart) {
//                // TODO: update order filling (?)
//            }

        }

        int id;
        int status = 0; /* created/accepted/cancelled/in process/completed 0/1/2/3/4 */
    };
}

#endif //RESTAURANTBES_ORDER_H
