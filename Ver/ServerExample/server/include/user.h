#pragma once

#include "fwd.h"

#include <corvusoft/restbed/session.hpp>
#include <folly/Synchronized.h>

#include <string>
#include <unordered_set>
#include <memory>
#include <queue>

namespace restbes {

struct User {
private:
    std::shared_ptr<Server> server;
    folly::Synchronized<std::string> id;
    folly::Synchronized<std::unordered_set<unsigned int>> activeSessions;

public:
    explicit User(std::string nm, std::shared_ptr<Server> serv);

    void push(std::shared_ptr<restbed::Response> response);

    void addSession(unsigned int session_id);

    void eraseInactiveSessions();

    [[nodiscard]] std::shared_ptr<Session>
    getSession(unsigned int session_id) const;

    [[nodiscard]] std::string getId() const;
};

} //restbes
