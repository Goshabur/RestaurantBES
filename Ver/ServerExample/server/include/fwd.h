#pragma once

namespace restbes {

namespace user_structure {
struct User;
} // user_structure

namespace server_structure {
enum Connection {
    KEEP_ALIVE,
    CLOSE
};

struct Server;
} // server_structure

namespace session_structure {
struct Session;
}

} // restbes
