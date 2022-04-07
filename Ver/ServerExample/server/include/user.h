#pragma once

#include "fwd.h"

#include <corvusoft/restbed/session.hpp>

#include <string>
#include <unordered_set>
#include <shared_mutex>
#include <memory>

namespace restbes::user_structure {

using restbed::Session;

struct User {
private:
    std::string name;
    std::unordered_set<std::shared_ptr<Session>> activeSessions;
    mutable std::shared_mutex mutex;

public:
    explicit User(std::string nm);

    void yieldMessage(const std::string &message, const std::string &path);

    void addSession(std::shared_ptr<Session> session);

    void eraseInactiveSessions();

    [[nodiscard]] std::shared_ptr<Session>
    getSession(std::shared_ptr<Session> session) const;

    [[nodiscard]] const std::unordered_set<std::shared_ptr<Session>> &
    getActiveSessions() const;

    [[nodiscard]] const std::string &getName() const;
};

} //user_structure
