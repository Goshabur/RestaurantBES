// TODO: в хедерах можно использовать pragma once

#ifndef RESTAURANTBES_FWD_H
#define RESTAURANTBES_FWD_H

namespace user_structure {
struct User;
}

namespace server_structure {
enum Connection {
    KEEP_ALIVE,
    CLOSE
};

struct Server;
}

#endif //RESTAURANTBES_FWD_H
