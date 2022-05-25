#pragma once

namespace restbes {

struct User;

enum Connection {
    KEEP_ALIVE,
    CLOSE
};

struct Server;

struct Session;

} // restbes
