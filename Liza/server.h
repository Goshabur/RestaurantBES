#ifndef RESTAURANTBES_SERVER_H
#define RESTAURANTBES_SERVER_H

#include <vector>
#include <string>

namespace db {
    void connect_to_db_exec(const std::string &sql);

    std::string connect_to_db_get(const std::string &sql);
}

#endif //RESTAURANTBES_SERVER_H
