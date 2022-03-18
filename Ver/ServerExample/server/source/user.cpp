#include "user.h"
#include "server.h"

namespace user_structure {

using server_structure::Connection;

User::User(std::string nm) : name(
        std::move(nm)) {}

void User::yieldMessage(const std::string &message, const std::string &path) {
    auto response = generateResponse(message, Connection::KEEP_ALIVE);
    std::unique_lock lock(mutex);
    for (const auto &session: activeSessions) {
        if (session->is_open() && path == session->get_request()->get_path()) {
            session->yield(*response);
        }
    }
}

void User::addSession(std::shared_ptr<Session> session) {
    std::unique_lock lock(mutex);
    activeSessions.insert(std::move(session));
}

void User::eraseInactiveSessions() {
    std::unique_lock lock(mutex);
    for (auto session = activeSessions.begin();
         session != activeSessions.end();) {
        if ((*session)->is_closed()) {
            session = activeSessions.erase(session);
        } else {
            ++session;
        }
    }
}

std::shared_ptr<Session>
User::getSession(std::shared_ptr<Session> session) const {
    std::shared_lock lock(mutex);
    if (activeSessions.count(session) == 0) return nullptr;
    else return session;
}

const std::unordered_set<std::shared_ptr<Session>> &
User::getActiveSessions() const {
    std::shared_lock lock(mutex);
    return activeSessions;
}

const std::string &User::getName() const {
    std::shared_lock lock(mutex);
    return name;
}

} //user_structure
