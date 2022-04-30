#pragma once

#include "fwd.h"

#include <corvusoft/restbed/session.hpp>
#include <folly/Synchronized.h>

#include <string>
#include <unordered_map>
#include <memory>
#include <queue>

namespace restbes::user_structure {

using session_structure::Session;

struct User {
private:
    folly::Synchronized<std::string> id;
    folly::Synchronized<std::unordered_map<std::shared_ptr<restbed::Session>, std::shared_ptr<Session>>> activeSessions;

public:
    explicit User(std::string nm);

    void pushMessage(std::shared_ptr<restbed::Response> response);

    void addSession(std::shared_ptr<restbed::Session> session);

    void eraseInactiveSessions();

    [[nodiscard]] std::shared_ptr<Session>
    getSession(std::shared_ptr<restbed::Session>) const;

    [[nodiscard]] std::unordered_map<std::shared_ptr<restbed::Session>, std::shared_ptr<Session>>
    getActiveSessions() const;

    [[nodiscard]] const std::string &getId() const;
};

} //user_structure
