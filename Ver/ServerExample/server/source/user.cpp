#include "user.h"
#include "server.h"

namespace restbes::user_structure {

using server_structure::Connection;

User::User(std::string nm) : name(
        std::move(nm)) {}

void User::yieldMessage(const std::string &message, const std::string &path) {
    auto response = generateResponse(message, "text/plain",
                                     Connection::KEEP_ALIVE);
    auto lockedSessions = activeSessions.rlock();
    for (const auto &session: *lockedSessions) {
        if (session->is_open() && path == session->get_request()->get_path()) {
            session->yield(*response);
        }
    }
}

void User::addSession(std::shared_ptr<Session> session) {
    activeSessions.wlock()->insert(std::move(session));
}

void User::eraseInactiveSessions() {
    auto lockedSessions = activeSessions.wlock();
    for (auto session = lockedSessions->begin();
         session != lockedSessions->end();) {
        if ((*session)->is_closed()) {
            session = lockedSessions->erase(session);
        } else {
            ++session;
        }
    }
}

std::shared_ptr<Session>
User::getSession(std::shared_ptr<Session> session) const {
    if (activeSessions.rlock()->count(session) == 0) return nullptr;
    else return session;
}

std::unordered_set<std::shared_ptr<Session>>
User::getActiveSessions() const {
    return *(activeSessions.rlock());
}

const std::string &User::getName() const {
    return *(name.rlock());
}

} //user_structure
